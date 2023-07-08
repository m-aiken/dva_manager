#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Controls/EQ/EqProperties.h"
#include "Controls/Compressor/CompressorProperties.h"
#include "Utilities/DvaInfo.h"
#include "Utilities/GlobalConstants.h"

/*---------------------------------------------------------------------------
**
*/
DvaManagerProcessor::DvaManagerProcessor()
    : juce::AudioProcessor(
        juce::AudioProcessor::BusesProperties().withOutput("Output", juce::AudioChannelSet::mono(), true))
    , apvts_(*this, nullptr, "CompressorParams", createParameterLayout())
{
    format_manager_.registerBasicFormats();

    initState();

    apvts_.addParameterListener(Comp::str(Comp::THRESHOLD), this);
    apvts_.addParameterListener(Comp::str(Comp::RATIO), this);
    apvts_.addParameterListener(Comp::str(Comp::ATTACK), this);
    apvts_.addParameterListener(Comp::str(Comp::RELEASE), this);
}

/*---------------------------------------------------------------------------
**
*/
DvaManagerProcessor::~DvaManagerProcessor()
{
}

/*---------------------------------------------------------------------------
**
*/
const juce::String
DvaManagerProcessor::getName() const
{
    return JucePlugin_Name;
}

/*---------------------------------------------------------------------------
**
*/
bool
DvaManagerProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

/*---------------------------------------------------------------------------
**
*/
bool
DvaManagerProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

/*---------------------------------------------------------------------------
**
*/
bool
DvaManagerProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

/*---------------------------------------------------------------------------
**
*/
double
DvaManagerProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

/*---------------------------------------------------------------------------
**
*/
int
DvaManagerProcessor::getNumPrograms()
{
    return 1;  // NB: some hosts don't cope very well if you tell them there are 0 programs,
               // so this should be at least 1, even if you're not really implementing programs.
}

/*---------------------------------------------------------------------------
**
*/
int
DvaManagerProcessor::getCurrentProgram()
{
    return 0;
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

/*---------------------------------------------------------------------------
**
*/
const juce::String
DvaManagerProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Pre-playback initialisation.
    transport_source_.prepareToPlay(samplesPerBlock, sampleRate);

    juce::dsp::ProcessSpec process_spec;
    process_spec.sampleRate       = sampleRate;
    process_spec.numChannels      = static_cast< juce::uint32 >(getTotalNumOutputChannels());
    process_spec.maximumBlockSize = static_cast< juce::uint32 >(samplesPerBlock);

    filter_chain_.prepare(process_spec);
    updateFilters(sampleRate);

    compressor_.prepare(process_spec);
    updateCompressor();

    fft_buffer_.prepare(sampleRate);

    peak_mono_post_.reset(sampleRate, 0.3);
    rms_mono_post_.reset(sampleRate, 0.3);
    peak_mono_post_.setCurrentAndTargetValue(Global::NEG_INF);
    rms_mono_post_.setCurrentAndTargetValue(Global::NEG_INF);

    lufs_filter_.prepare(process_spec);

    //    comp_makeup_gain_.prepare(process_spec);
    //    comp_makeup_gain_.setRampDurationSeconds(0.05);

#if TEST_TONE
    analyser_test_tone_.prepare(process_spec);
    analyser_test_tone_.initialise([](float x) { return std::sin(x); });
    analyser_test_tone_.setFrequency(1000.f);
#endif
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    transport_source_.releaseResources();
}

/*---------------------------------------------------------------------------
**
*/
bool
DvaManagerProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
    #if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    #endif

    return true;
#endif
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::processBlock(juce::AudioBuffer< float >& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto                    totalNumInputChannels  = getTotalNumInputChannels();
    auto                    totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    // Transport.
    AudioSourceChannelInfo audio_source_channel_info(buffer);

    transport_source_.getNextAudioBlock(audio_source_channel_info);

    // Process Context.
    juce::dsp::AudioBlock< float >              audio_block(buffer);
    juce::dsp::ProcessContextReplacing< float > process_context(audio_block);

#if TEST_TONE
    // Analyser test tone.
    analyser_test_tone_.process(process_context);
#endif

    // Analyser.
    auto* channel_data = buffer.getReadPointer(0);
    for (auto i = 0; i < buffer.getNumSamples(); ++i) {
        fft_buffer_.pushNextSample(channel_data[i]);
    }

    // Compressor.
    auto comp_enabled = dynamic_cast< juce::AudioParameterBool* >(apvts_.getParameter(Comp::str(Comp::ENABLED)));
    if (comp_enabled != nullptr && comp_enabled->get()) {

        auto input_gain = dynamic_cast< juce::AudioParameterFloat* >(apvts_.getParameter(Comp::str(Comp::INPUT_GAIN)));
        if (input_gain != nullptr) {
            buffer.applyGain(input_gain->convertTo0to1(input_gain->get()));
        }

        compressor_.process(process_context);

        auto makeup_gain = dynamic_cast< juce::AudioParameterFloat* >(apvts_.getParameter(Comp::str(Comp::MAKEUP_GAIN)));
        if (makeup_gain != nullptr) {
            buffer.applyGain(makeup_gain->convertTo0to1(makeup_gain->get()));
        }
    }

    // EQ.
    auto eq_enabled = dynamic_cast< juce::AudioParameterBool* >(apvts_.getParameter(Eq::str(Eq::ENABLED)));
    if (eq_enabled != nullptr && eq_enabled->get()) {
        updateFilters(getSampleRate());
        filter_chain_.process(process_context);
    }

    // Peak, RMS and LUFS values (post EQ and compressor processing).
    getMonoPeak(buffer);
    getMonoRms(buffer);
    lufs_mono_post_ = lufs_filter_.getMonoLufs(buffer);

#if SILENCE_OUTPUT
    for (auto i = 0; i < buffer.getNumSamples(); ++i) {
        buffer.setSample(0, i, 0.f);
    }
#endif
}

/*---------------------------------------------------------------------------
**
*/
bool
DvaManagerProcessor::hasEditor() const
{
    return true;  // (change this to false if you choose to not supply an editor)
}

/*---------------------------------------------------------------------------
**
*/
juce::AudioProcessorEditor*
DvaManagerProcessor::createEditor()
{
    return new DvaManagerEditor(*this);
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::ignoreUnused(destData);
    //    juce::MemoryOutputStream output_stream(destData, true);
    //    state_.writeToStream(output_stream);
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::ignoreUnused(data, sizeInBytes);
    //    auto saved_state = juce::ValueTree::readFromData(data, sizeInBytes);
    //
    //    if (saved_state.isValid()) {
    //        state_ = saved_state;
    //    }
}

/*---------------------------------------------------------------------------
**
*/
/*static*/ juce::AudioProcessorValueTreeState::ParameterLayout
DvaManagerProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    //====================================================================================
    // EQ Parameters.
    //====================================================================================

    // Global enabled/disabled.
    layout.add(std::make_unique< juce::AudioParameterBool >(Eq::str(Eq::ENABLED), Eq::str(Eq::ENABLED), true));

    //====================================================================================
    // Cut band: low cut
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::LOW_CUT_FREQ),
                                                             Eq::str(Eq::LOW_CUT_FREQ),
                                                             Eq::LOW_CUT_FREQ_RANGE,
                                                             50.f));
    layout.add(std::make_unique< juce::AudioParameterChoice >(Eq::str(Eq::LOW_CUT_SLOPE),
                                                              Eq::str(Eq::LOW_CUT_SLOPE),
                                                              Eq::SLOPE_CHOICES,
                                                              3));

    layout.add(
        std::make_unique< juce::AudioParameterBool >(Eq::str(Eq::LOW_CUT_ENABLED), Eq::str(Eq::LOW_CUT_ENABLED), false));

    //====================================================================================
    // Peak band: low frequency
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::LF_FREQ),
                                                             Eq::str(Eq::LF_FREQ),
                                                             Eq::LF_FREQ_RANGE,
                                                             200.f));
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::LF_GAIN),
                                                             Eq::str(Eq::LF_GAIN),
                                                             Eq::PEAK_GAIN_RANGE,
                                                             0.f));
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::LF_QUALITY),
                                                             Eq::str(Eq::LF_QUALITY),
                                                             Eq::PEAK_Q_RANGE,
                                                             1.f));

    layout.add(std::make_unique< juce::AudioParameterBool >(Eq::str(Eq::LF_ENABLED), Eq::str(Eq::LF_ENABLED), true));

    //====================================================================================
    // Peak band: low mid frequency
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::LMF_FREQ),
                                                             Eq::str(Eq::LMF_FREQ),
                                                             Eq::LMF_FREQ_RANGE,
                                                             900.f));
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::LMF_GAIN),
                                                             Eq::str(Eq::LMF_GAIN),
                                                             Eq::PEAK_GAIN_RANGE,
                                                             0.f));
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::LMF_QUALITY),
                                                             Eq::str(Eq::LMF_QUALITY),
                                                             Eq::PEAK_Q_RANGE,
                                                             1.f));

    layout.add(std::make_unique< juce::AudioParameterBool >(Eq::str(Eq::LMF_ENABLED), Eq::str(Eq::LMF_ENABLED), true));

    //====================================================================================
    // Peak band: high mid frequency
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::HMF_FREQ),
                                                             Eq::str(Eq::HMF_FREQ),
                                                             Eq::HMF_FREQ_RANGE,
                                                             3000.f));
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::HMF_GAIN),
                                                             Eq::str(Eq::HMF_GAIN),
                                                             Eq::PEAK_GAIN_RANGE,
                                                             0.f));
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::HMF_QUALITY),
                                                             Eq::str(Eq::HMF_QUALITY),
                                                             Eq::PEAK_Q_RANGE,
                                                             1.f));

    layout.add(std::make_unique< juce::AudioParameterBool >(Eq::str(Eq::HMF_ENABLED), Eq::str(Eq::HMF_ENABLED), true));

    //====================================================================================
    // Peak band: high frequency
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::HF_FREQ),
                                                             Eq::str(Eq::HF_FREQ),
                                                             Eq::HF_FREQ_RANGE,
                                                             7500.f));
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::HF_GAIN),
                                                             Eq::str(Eq::HF_GAIN),
                                                             Eq::PEAK_GAIN_RANGE,
                                                             0.f));
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::HF_QUALITY),
                                                             Eq::str(Eq::HF_QUALITY),
                                                             Eq::PEAK_Q_RANGE,
                                                             1.f));

    layout.add(std::make_unique< juce::AudioParameterBool >(Eq::str(Eq::HF_ENABLED), Eq::str(Eq::HF_ENABLED), true));

    //====================================================================================
    // Cut band: high cut
    layout.add(std::make_unique< juce::AudioParameterFloat >(Eq::str(Eq::HIGH_CUT_FREQ),
                                                             Eq::str(Eq::HIGH_CUT_FREQ),
                                                             Eq::HIGH_CUT_FREQ_RANGE,
                                                             15000.f));
    layout.add(std::make_unique< juce::AudioParameterChoice >(Eq::str(Eq::HIGH_CUT_SLOPE),
                                                              Eq::str(Eq::HIGH_CUT_SLOPE),
                                                              Eq::SLOPE_CHOICES,
                                                              3));

    layout.add(
        std::make_unique< juce::AudioParameterBool >(Eq::str(Eq::HIGH_CUT_ENABLED), Eq::str(Eq::HIGH_CUT_ENABLED), false));

    //====================================================================================
    // Compressor parameters.
    //====================================================================================
    layout.add(std::make_unique< juce::AudioParameterBool >(Comp::str(Comp::ENABLED), Comp::str(Comp::ENABLED), false));

    layout.add(std::make_unique< juce::AudioParameterFloat >(Comp::str(Comp::THRESHOLD),
                                                             Comp::str(Comp::THRESHOLD),
                                                             Comp::THRESHOLD_RANGE,
                                                             0.f));
    layout.add(std::make_unique< juce::AudioParameterFloat >(Comp::str(Comp::INPUT_GAIN),
                                                             Comp::str(Comp::INPUT_GAIN),
                                                             Comp::GAIN_RANGE,
                                                             0.f));
    layout.add(std::make_unique< juce::AudioParameterChoice >(Comp::str(Comp::RATIO),
                                                              Comp::str(Comp::RATIO),
                                                              Comp::RATIO_CHOICES,
                                                              0));
    layout.add(std::make_unique< juce::AudioParameterChoice >(Comp::str(Comp::ATTACK),
                                                              Comp::str(Comp::ATTACK),
                                                              Comp::ATTACK_CHOICES,
                                                              2));
    layout.add(std::make_unique< juce::AudioParameterChoice >(Comp::str(Comp::RELEASE),
                                                              Comp::str(Comp::RELEASE),
                                                              Comp::RELEASE_CHOICES,
                                                              2));
    layout.add(std::make_unique< juce::AudioParameterFloat >(Comp::str(Comp::MAKEUP_GAIN),
                                                             Comp::str(Comp::MAKEUP_GAIN),
                                                             Comp::GAIN_RANGE,
                                                             0.f));

    return layout;
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::parameterChanged(const juce::String& parameter_id, float new_value)
{
    juce::ignoreUnused(parameter_id, new_value);

    // TODO refactor
    updateCompressor();
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::updateCompressor()
{
    // TODO refactor

    auto threshold = dynamic_cast< juce::AudioParameterFloat* >(apvts_.getParameter(Comp::str(Comp::THRESHOLD)));
    if (threshold != nullptr) {
        compressor_.setThreshold(threshold->get());
    }

    auto ratio = dynamic_cast< juce::AudioParameterChoice* >(apvts_.getParameter(Comp::str(Comp::RATIO)));
    if (ratio != nullptr) {
        compressor_.setRatio(ratio->getCurrentChoiceName().getFloatValue());
    }

    auto attack = dynamic_cast< juce::AudioParameterChoice* >(apvts_.getParameter(Comp::str(Comp::ATTACK)));
    if (attack != nullptr) {
        compressor_.setAttack(attack->getCurrentChoiceName().getFloatValue());
    }

    auto release = dynamic_cast< juce::AudioParameterChoice* >(apvts_.getParameter(Comp::str(Comp::RELEASE)));
    if (release != nullptr) {
        compressor_.setRelease(release->getCurrentChoiceName().getFloatValue());
    }
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::initState()
{
    if (!state_.isValid()) {
        return;
    }

    state_.setProperty("selected_dva", "1", nullptr);

    namespace DI = DvaInfo;

    for (int i = 0; i < MAX_DVAS; ++i) {
        auto num = juce::String(i + 1);

        state_.addChild({ "dva_" + num,
                          {
                              { DI::getXmlProperty(DI::DVA_NUM), num },
                              { DI::getXmlProperty(DI::FILENAME), "No file selected..." },
                              { DI::getXmlProperty(DI::PATH), "" },
                              { DI::getXmlProperty(DI::SIZE), "0" },
                              { DI::getXmlProperty(DI::DURATION_SECONDS), "0" },
                              { DI::getXmlProperty(DI::NUM_CHANNELS), "0" },
                              { DI::getXmlProperty(DI::SAMPLE_RATE), "0.0" },
                          } },
                        i,
                        nullptr);
    }
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::updateState(juce::ValueTree& dva, juce::File& file)
{
    if (!dva.isValid()) {
        return;
    }

    auto* reader = format_manager_.createReaderFor(file);

    if (reader == nullptr) {
        return;
    }

    namespace DI = DvaInfo;

    if (dva.hasProperty(DI::getXmlProperty(DI::FILENAME))) {
        dva.setProperty(DI::getXmlProperty(DI::FILENAME), file.getFileName(), nullptr);
    }

    if (dva.hasProperty(DI::getXmlProperty(DI::PATH))) {
        dva.setProperty(DI::getXmlProperty(DI::PATH), file.getFullPathName(), nullptr);
    }

    if (dva.hasProperty(DI::getXmlProperty(DI::SIZE))) {
        dva.setProperty(DI::getXmlProperty(DI::SIZE),
                        juce::String(static_cast< double >(file.getSize()) / 1000000),
                        nullptr);
    }

    if (dva.hasProperty(DI::getXmlProperty(DI::DURATION_SECONDS))) {
        dva.setProperty(DI::getXmlProperty(DI::DURATION_SECONDS),
                        juce::String(reader->lengthInSamples / reader->sampleRate),
                        nullptr);
    }

    if (dva.hasProperty(DI::getXmlProperty(DI::NUM_CHANNELS))) {
        dva.setProperty(DI::getXmlProperty(DI::NUM_CHANNELS), juce::String(reader->numChannels), nullptr);
    }

    if (dva.hasProperty(DI::getXmlProperty(DI::SAMPLE_RATE))) {
        dva.setProperty(DI::getXmlProperty(DI::SAMPLE_RATE), juce::String(reader->sampleRate), nullptr);
    }

    delete reader;
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::setTransportSource()
{
    if (!state_.isValid()) {
        return;
    }

    auto            selected_dva = state_.getProperty("selected_dva").toString();
    juce::ValueTree dva          = state_.getChildWithName("dva_" + selected_dva);

    if (!dva.isValid() || !dva.hasProperty(DvaInfo::getXmlProperty(DvaInfo::PATH))) {
        return;
    }

    juce::File file(dva.getProperty(DvaInfo::getXmlProperty(DvaInfo::PATH)));

    if (file == juce::File {}) {
        transport_source_.setSource(nullptr);

        return;
    }

    auto* reader = format_manager_.createReaderFor(file);

    if (reader == nullptr) {
        return;
    }

    auto new_source = std::make_unique< juce::AudioFormatReaderSource >(reader, true);

    transport_source_.setSource(new_source.get(), 0, nullptr, reader->sampleRate, 1);

    reader_source_.reset(new_source.release());

    updateState(dva, file);
}

/*---------------------------------------------------------------------------
**
*/
float
DvaManagerProcessor::getPeakValue()
{
    return peak_mono_post_.getCurrentValue();
}

/*---------------------------------------------------------------------------
**
*/
float
DvaManagerProcessor::getRmsValue()
{
    return rms_mono_post_.getCurrentValue();
}

/*---------------------------------------------------------------------------
**
*/
float
DvaManagerProcessor::getLufsValue()
{
    return lufs_mono_post_;
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::updateFilters(double sample_rate)
{
    updateLowCutFilter(sample_rate);
    updateHighCutFilter(sample_rate);

    // Bands.
    updateBandFilter(Eq::LF_FREQ, Eq::LF_GAIN, Eq::LF_QUALITY, Eq::LF_ENABLED, FilterDsp::ChainPositions::LF, sample_rate);
    updateBandFilter(Eq::LMF_FREQ, Eq::LMF_GAIN, Eq::LMF_QUALITY, Eq::LMF_ENABLED, FilterDsp::ChainPositions::LMF, sample_rate);
    updateBandFilter(Eq::HMF_FREQ, Eq::HMF_GAIN, Eq::HMF_QUALITY, Eq::HMF_ENABLED, FilterDsp::ChainPositions::HMF, sample_rate);
    updateBandFilter(Eq::HF_FREQ, Eq::HF_GAIN, Eq::HF_QUALITY, Eq::HF_ENABLED, FilterDsp::ChainPositions::HF, sample_rate);
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::updateLowCutFilter(double sample_rate)
{
    auto enabled = dynamic_cast< juce::AudioParameterBool* >(apvts_.getParameter(Eq::str(Eq::LOW_CUT_ENABLED)));

    if (enabled == nullptr || !enabled->get()) {
        return;
    }

    auto freq  = dynamic_cast< juce::AudioParameterFloat* >(apvts_.getParameter(Eq::str(Eq::LOW_CUT_FREQ)));
    auto slope = dynamic_cast< juce::AudioParameterChoice* >(apvts_.getParameter(Eq::str(Eq::LOW_CUT_SLOPE)));

    if ((freq != nullptr) && (slope != nullptr)) {
        FilterDsp::Coefficients coefficients = FilterDsp::makeHighPassFilter(freq, slope, sample_rate);
        FilterDsp::updateCutCoefficients(filter_chain_, FilterDsp::ChainPositions::LOW_CUT, coefficients, slope);
    }
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::updateHighCutFilter(double sample_rate)
{
    auto enabled = dynamic_cast< juce::AudioParameterBool* >(apvts_.getParameter(Eq::str(Eq::HIGH_CUT_ENABLED)));

    if (enabled == nullptr || !enabled->get()) {
        return;
    }

    auto freq  = dynamic_cast< juce::AudioParameterFloat* >(apvts_.getParameter(Eq::str(Eq::HIGH_CUT_FREQ)));
    auto slope = dynamic_cast< juce::AudioParameterChoice* >(apvts_.getParameter(Eq::str(Eq::HIGH_CUT_SLOPE)));

    if ((freq != nullptr) && (slope != nullptr)) {
        FilterDsp::Coefficients coefficients = FilterDsp::makeLowPassFilter(freq, slope, sample_rate);
        FilterDsp::updateCutCoefficients(filter_chain_, FilterDsp::ChainPositions::HIGH_CUT, coefficients, slope);
    }
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::updateBandFilter(Eq::Control               freq_ctrl,
                                      Eq::Control               gain_ctrl,
                                      Eq::Control               q_ctrl,
                                      Eq::Control               enabled_ctrl,
                                      FilterDsp::ChainPositions chain_position,
                                      double                    sample_rate)
{
    auto enabled = dynamic_cast< juce::AudioParameterBool* >(apvts_.getParameter(Eq::str(enabled_ctrl)));

    if (enabled == nullptr || !enabled->get()) {
        return;
    }

    auto freq = dynamic_cast< juce::AudioParameterFloat* >(apvts_.getParameter(Eq::str(freq_ctrl)));
    auto gain = dynamic_cast< juce::AudioParameterFloat* >(apvts_.getParameter(Eq::str(gain_ctrl)));
    auto q    = dynamic_cast< juce::AudioParameterFloat* >(apvts_.getParameter(Eq::str(q_ctrl)));

    if ((freq != nullptr) && (gain != nullptr) && (q != nullptr)) {
        FilterDsp::updatePeakCoefficients(filter_chain_, chain_position, freq, q, gain, sample_rate);
    }
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::getMonoPeak(juce::AudioBuffer< float >& buffer)
{
    auto mag    = buffer.getMagnitude(0, 0, buffer.getNumSamples());
    auto mag_db = juce::Decibels::gainToDecibels(mag, Global::NEG_INF);

    peak_mono_post_.skip(buffer.getNumSamples());

    if (mag_db < peak_mono_post_.getCurrentValue()) {
        peak_mono_post_.setTargetValue(mag_db);
    }
    else {
        peak_mono_post_.setCurrentAndTargetValue(mag_db);
    }
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerProcessor::getMonoRms(juce::AudioBuffer< float >& buffer)
{
    auto rms    = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    auto rms_db = juce::Decibels::gainToDecibels(rms, Global::NEG_INF);

    rms_mono_post_.skip(buffer.getNumSamples());

    if (rms_db < rms_mono_post_.getCurrentValue()) {
        rms_mono_post_.setTargetValue(rms_db);
    }
    else {
        rms_mono_post_.setCurrentAndTargetValue(rms_db);
    }
}

/*---------------------------------------------------------------------------
**
*/
// This creates new instances of the plugin.
juce::AudioProcessor* JUCE_CALLTYPE
createPluginFilter()
{
    return new DvaManagerProcessor();
}

/*---------------------------------------------------------------------------
** End of File
*/
