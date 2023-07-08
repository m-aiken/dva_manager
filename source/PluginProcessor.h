#pragma once

#include <JuceHeader.h>

#include "Analyser/FFTBuffer.h"
#include "Controls/EQ/EqProperties.h"
#include "FilterDsp/FilterDsp.h"
#include "LUFS/LufsFilter.h"
#include "Utilities/GlobalConstants.h"

#define TEST_TONE      false
#define SILENCE_OUTPUT true

//==============================================================================
class DvaManagerProcessor
    : public juce::AudioProcessor
    , public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    DvaManagerProcessor();
    ~DvaManagerProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer< float >&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool                        hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool   acceptsMidi() const override;
    bool   producesMidi() const override;
    bool   isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int                getNumPrograms() override;
    int                getCurrentProgram() override;
    void               setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void               changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState                         apvts_;
    void parameterChanged(const juce::String& parameter_id, float new_value) override;

    void updateCompressor();

    enum { MAX_DVAS = 64 };
    juce::ValueTree state_ { "dva_data" };
    void            initState();
    void            updateState(juce::ValueTree& dva, juce::File& file);

    juce::AudioFormatManager&      getFormatManager() { return format_manager_; }
    juce::AudioFormatReaderSource* getReaderSource() { return reader_source_.get(); }
    juce::AudioTransportSource&    getTransportSource() { return transport_source_; }
    void                           setTransportSource();

    FFTBuffer fft_buffer_;

    float getPeakValue();
    float getRmsValue();
    float getLufsValue();

    FilterDsp::MonoChain& getFilterChain() { return filter_chain_; }

private:
    juce::AudioFormatManager                         format_manager_;
    std::unique_ptr< juce::AudioFormatReaderSource > reader_source_;
    juce::AudioTransportSource                       transport_source_;

    FilterDsp::MonoChain filter_chain_;
    void                 updateFilters(double sample_rate);
    void                 updateLowCutFilter(double sample_rate);
    void                 updateHighCutFilter(double sample_rate);
    void                 updateBandFilter(Eq::Control               freq_ctrl,
                                          Eq::Control               gain_ctrl,
                                          Eq::Control               q_ctrl,
                                          Eq::Control               enabled_ctrl,
                                          FilterDsp::ChainPositions chain_position,
                                          double                    sample_rate);

    juce::dsp::Compressor< float > compressor_;
    //    juce::dsp::Gain< float >       comp_makeup_gain_;

    juce::SmoothedValue< float, juce::ValueSmoothingTypes::Linear > peak_mono_post_;
    juce::SmoothedValue< float, juce::ValueSmoothingTypes::Linear > rms_mono_post_;
    float                                                           lufs_mono_post_ { 0.f };

    void getMonoPeak(juce::AudioBuffer< float >& buffer);
    void getMonoRms(juce::AudioBuffer< float >& buffer);

    LufsFilter lufs_filter_;

#if TEST_TONE
    juce::dsp::Oscillator< float > analyser_test_tone_;
#endif

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DvaManagerProcessor)
};
