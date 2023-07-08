#include "ResponseCurve.h"
#include "../Controls/EQ/EqProperties.h"
#include "../Utilities/GlobalConstants.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
ResponseCurve::ResponseCurve(FilterDsp::MonoChain&               filter_chain,
                             double                              sample_rate,
                             juce::AudioProcessorValueTreeState& apvts)
    : filter_chain_(filter_chain)
    , sample_rate_(sample_rate)
    , should_repaint_(false)
    , apvts_(apvts)
{
    nodes_.at(LF)  = std::make_unique< AnalyserNode >(apvts_, Eq::LF_ENABLED);
    nodes_.at(LMF) = std::make_unique< AnalyserNode >(apvts_, Eq::LMF_ENABLED);
    nodes_.at(HMF) = std::make_unique< AnalyserNode >(apvts_, Eq::HMF_ENABLED);
    nodes_.at(HF)  = std::make_unique< AnalyserNode >(apvts_, Eq::HF_ENABLED);

    for (size_t i = 0; i < NUM_NODES; ++i) {
        addAndMakeVisible(*nodes_.at(i));
    }

    node_coordinates_.at(LF).first.referTo(apvts_.getParameterAsValue(Eq::str(Eq::LF_FREQ)));
    node_coordinates_.at(LF).second.referTo(apvts_.getParameterAsValue(Eq::str(Eq::LF_GAIN)));

    node_coordinates_.at(LMF).first.referTo(apvts_.getParameterAsValue(Eq::str(Eq::LMF_FREQ)));
    node_coordinates_.at(LMF).second.referTo(apvts_.getParameterAsValue(Eq::str(Eq::LMF_GAIN)));

    node_coordinates_.at(HMF).first.referTo(apvts_.getParameterAsValue(Eq::str(Eq::HMF_FREQ)));
    node_coordinates_.at(HMF).second.referTo(apvts_.getParameterAsValue(Eq::str(Eq::HMF_GAIN)));

    node_coordinates_.at(HF).first.referTo(apvts_.getParameterAsValue(Eq::str(Eq::HF_FREQ)));
    node_coordinates_.at(HF).second.referTo(apvts_.getParameterAsValue(Eq::str(Eq::HF_GAIN)));

    addParameterListeners();

    startTimerHz(60);
}

/*---------------------------------------------------------------------------
**
*/
ResponseCurve::~ResponseCurve()
{
    stopTimer();

    removeParameterListeners();
}

/*---------------------------------------------------------------------------
**
*/
void
ResponseCurve::paint(juce::Graphics& g)
{
    if (magnitudes_.size() == 0) {
        return;
    }

    auto       bounds        = getLocalBounds();
    auto       bounds_bottom = bounds.getBottom();
    auto       bounds_top    = bounds.getY();
    auto       alpha         = groupIsEnabled() ? 0.3f : 0.1f;
    juce::Path path;

    g.setColour(TailwindColours::Amber50.withAlpha(alpha));

    path.startNewSubPath(0, mapMagnitudeToY(magnitudes_.at(0), bounds_bottom, bounds_top));

    for (auto i = 1; i < magnitudes_.size(); ++i) {
        path.lineTo(i, mapMagnitudeToY(magnitudes_.at(i), bounds_bottom, bounds_top));
    }

    g.strokePath(path, juce::PathStrokeType(2.f));

    // Nodes.
    if (nodes_.at(LF).get() != nullptr) {
        auto coords = getNodeCoordinates(Eq::LF_FREQ);
        nodes_.at(LF).get()->setBounds(coords.getX(), coords.getY(), NODE_DIAMETER, NODE_DIAMETER);
    }

    if (nodes_.at(LMF).get() != nullptr) {
        auto coords = getNodeCoordinates(Eq::LMF_FREQ);
        nodes_.at(LMF).get()->setBounds(coords.getX(), coords.getY(), NODE_DIAMETER, NODE_DIAMETER);
    }

    if (nodes_.at(HMF).get() != nullptr) {
        auto coords = getNodeCoordinates(Eq::HMF_FREQ);
        nodes_.at(HMF).get()->setBounds(coords.getX(), coords.getY(), NODE_DIAMETER, NODE_DIAMETER);
    }

    if (nodes_.at(HF).get() != nullptr) {
        auto coords = getNodeCoordinates(Eq::HF_FREQ);
        nodes_.at(HF).get()->setBounds(coords.getX(), coords.getY(), NODE_DIAMETER, NODE_DIAMETER);
    }
}

/*---------------------------------------------------------------------------
**
*/
void
ResponseCurve::resized()
{
    // Initialise the magnitudes vector.
    if (magnitudes_.size() == 0) {
        calculateMagnitudes();
    }
}

/*---------------------------------------------------------------------------
**
*/
void
ResponseCurve::mouseDrag(const juce::MouseEvent& event)
{
    auto bounds        = getLocalBounds();
    auto bounds_width  = bounds.getWidth();
    auto bounds_height = bounds.getHeight();

    auto coords = event.getPosition();
    auto x      = juce::jlimit< int >(0, bounds_width, coords.getX());
    auto y      = juce::jlimit< int >(0, bounds_height, coords.getY());
    auto x_hz   = juce::mapToLog10< float >(static_cast< float >(x) / bounds_width, Global::MIN_HZ, Global::MAX_HZ);
    auto y_db   = juce::jmap< float >(y, 0, bounds_height, Global::MAX_DB, Global::NEG_INF);

    // Get the closest node to the drag event (initially just by the x coordinate).
    NODE_POS closest_node;
    auto     shortest_distance = bounds_width;

    for (size_t i = 0; i < NUM_NODES; ++i) {
        AnalyserNode* node = nodes_.at(i).get();

        if (node == nullptr) {
            continue;
        }

        auto x_distance = std::abs(node->getX() - x);
        if (x_distance < shortest_distance) {
            closest_node      = static_cast< NODE_POS >(i);
            shortest_distance = x_distance;
        }
    }

    if (nodes_.at(closest_node).get() != nullptr && nodes_.at(closest_node).get()->getToggleState()) {
        node_coordinates_.at(closest_node).first.setValue(x_hz);
        node_coordinates_.at(closest_node).second.setValue(y_db);
    }
}

/*---------------------------------------------------------------------------
**
*/
void
ResponseCurve::parameterValueChanged(int parameter_index, float new_value)
{
    //    DBG("Index: " + juce::String(parameter_index) + ", Value: " + juce::String(new_value));
    juce::ignoreUnused(parameter_index, new_value);
    should_repaint_.set(true);
}

/*---------------------------------------------------------------------------
**
*/
void
ResponseCurve::parameterGestureChanged(int parameter_index, bool gesture_is_starting)
{
    juce::ignoreUnused(parameter_index, gesture_is_starting);
}

/*---------------------------------------------------------------------------
**
*/
void
ResponseCurve::timerCallback()
{
    if (should_repaint_.compareAndSetBool(false, true)) {
        calculateMagnitudes();
        repaint();
    }
}

/*---------------------------------------------------------------------------
**
*/
void
ResponseCurve::calculateMagnitudes()
{
    auto num_x_pixels = getLocalBounds().getWidth();

    auto& lc  = filter_chain_.get< FilterDsp::LOW_CUT >();
    auto& lf  = filter_chain_.get< FilterDsp::LF >();
    auto& lmf = filter_chain_.get< FilterDsp::LMF >();
    auto& hmf = filter_chain_.get< FilterDsp::HMF >();
    auto& hf  = filter_chain_.get< FilterDsp::HF >();
    auto& hc  = filter_chain_.get< FilterDsp::HIGH_CUT >();

    // Reset the magnitudes vector (zero it out).
    if (magnitudes_.size() != static_cast< size_t >(num_x_pixels)) {
        magnitudes_.resize(num_x_pixels);
    }

    std::fill(magnitudes_.begin(), magnitudes_.end(), 0.0);

    for (auto i = 0; i < num_x_pixels; ++i) {
        double mag = 1.0;

        if (groupIsEnabled()) {
            double freq = juce::mapToLog10< double >(static_cast< double >(i) / static_cast< double >(num_x_pixels),
                                                     Global::MIN_HZ,
                                                     Global::MAX_HZ);

            // Low Cut.
            if (bandIsEnabled(Eq::LOW_CUT_ENABLED)) {
                mag *= lc.get< FilterDsp::SLOPE_12 >().coefficients->getMagnitudeForFrequency(freq, sample_rate_);
                mag *= lc.get< FilterDsp::SLOPE_24 >().coefficients->getMagnitudeForFrequency(freq, sample_rate_);
                mag *= lc.get< FilterDsp::SLOPE_36 >().coefficients->getMagnitudeForFrequency(freq, sample_rate_);
                mag *= lc.get< FilterDsp::SLOPE_48 >().coefficients->getMagnitudeForFrequency(freq, sample_rate_);
            }

            // Peak Bands.
            if (bandIsEnabled(Eq::LF_ENABLED)) {
                mag *= lf.coefficients->getMagnitudeForFrequency(freq, sample_rate_);
            }

            if (bandIsEnabled(Eq::LMF_ENABLED)) {
                mag *= lmf.coefficients->getMagnitudeForFrequency(freq, sample_rate_);
            }

            if (bandIsEnabled(Eq::HMF_ENABLED)) {
                mag *= hmf.coefficients->getMagnitudeForFrequency(freq, sample_rate_);
            }

            if (bandIsEnabled(Eq::HF_ENABLED)) {
                mag *= hf.coefficients->getMagnitudeForFrequency(freq, sample_rate_);
            }

            // High Cut.
            if (bandIsEnabled(Eq::HIGH_CUT_ENABLED)) {
                mag *= hc.get< FilterDsp::SLOPE_12 >().coefficients->getMagnitudeForFrequency(freq, sample_rate_);
                mag *= hc.get< FilterDsp::SLOPE_24 >().coefficients->getMagnitudeForFrequency(freq, sample_rate_);
                mag *= hc.get< FilterDsp::SLOPE_36 >().coefficients->getMagnitudeForFrequency(freq, sample_rate_);
                mag *= hc.get< FilterDsp::SLOPE_48 >().coefficients->getMagnitudeForFrequency(freq, sample_rate_);
            }
        }

        magnitudes_.at(i) = juce::Decibels::gainToDecibels< double >(mag, Global::NEG_INF);
    }
}

/*---------------------------------------------------------------------------
**
*/
double
ResponseCurve::mapMagnitudeToY(double val, int bottom_y, int top_y)
{
    return juce::jmap< double >(val, Global::NEG_INF, Global::MAX_DB, bottom_y, top_y);
}

/*---------------------------------------------------------------------------
**
*/
juce::Point< int >
ResponseCurve::getNodeCoordinates(Eq::Control freq_control)
{
    juce::Point< int > coordinates(0, 0);
    auto               bounds       = getLocalBounds();
    auto               bounds_width = bounds.getWidth();

    auto freq_param = dynamic_cast< juce::AudioParameterFloat* >(apvts_.getParameter(Eq::str(freq_control)));
    if (freq_param != nullptr) {
        auto drawable_freq = juce::mapFromLog10(freq_param->get(), Global::MIN_HZ, Global::MAX_HZ);
        auto init_x        = static_cast< int >(std::floor(drawable_freq * bounds_width));

        // Clamp the upper bounds of the x coordinate so we don't get an out of bounds exception.
        auto node_x = juce::jmin< int >(init_x, bounds_width + NODE_RADIUS);

        if (node_x <= magnitudes_.size()) {
            auto node_y = mapMagnitudeToY(magnitudes_.at(node_x), bounds.getBottom(), bounds.getY());

            coordinates.setXY(node_x - NODE_RADIUS, node_y - NODE_RADIUS);
        }
    }

    return coordinates;
}

/*---------------------------------------------------------------------------
**
*/
bool
ResponseCurve::groupIsEnabled()
{
    juce::RangedAudioParameter* rap   = apvts_.getParameter(Eq::str(Eq::ENABLED));
    juce::AudioParameterBool*   param = (rap != nullptr) ? dynamic_cast< juce::AudioParameterBool* >(rap) : nullptr;

    if (param != nullptr) {
        return param->get();
    }

    return false;
}

/*---------------------------------------------------------------------------
**
*/
bool
ResponseCurve::bandIsEnabled(Eq::Control enabled_control)
{
    juce::RangedAudioParameter* rap   = apvts_.getParameter(Eq::str(enabled_control));
    juce::AudioParameterBool*   param = (rap != nullptr) ? dynamic_cast< juce::AudioParameterBool* >(rap) : nullptr;

    if (param != nullptr) {
        return param->get();
    }

    return false;
}

/*---------------------------------------------------------------------------
**
*/
void
ResponseCurve::addParameterListeners()
{
    for (auto& id : Eq::PARAM_IDS) {
        auto param = apvts_.getParameter(id);

        if (param != nullptr) {
            param->addListener(this);
        }
    }
}

/*---------------------------------------------------------------------------
**
*/
void
ResponseCurve::removeParameterListeners()
{
    for (auto& id : Eq::PARAM_IDS) {
        auto param = apvts_.getParameter(id);

        if (param != nullptr) {
            param->removeListener(this);
        }
    }
}

/*---------------------------------------------------------------------------
** End of File
*/
