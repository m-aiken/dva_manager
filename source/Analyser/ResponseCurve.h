#pragma once

#include <JuceHeader.h>

#include "../Controls/EQ/EqProperties.h"
#include "../FilterDsp/FilterDsp.h"
#include "../Widgets/AnalyserNode.h"

//==============================================================================
class ResponseCurve
    : public juce::Component
    , public juce::AudioProcessorParameter::Listener
    , public juce::Timer
{
public:
    ResponseCurve(FilterDsp::MonoChain& filter_chain, double sample_rate, juce::AudioProcessorValueTreeState& apvts);
    ~ResponseCurve();

    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDrag(const juce::MouseEvent& event) override;

    void parameterValueChanged(int parameter_index, float new_value) override;
    void parameterGestureChanged(int parameter_index, bool gesture_is_starting) override;

    void timerCallback() override;

    void               calculateMagnitudes();
    double             mapMagnitudeToY(double val, int bottom_y, int top_y);
    juce::Point< int > getNodeCoordinates(Eq::Control freq_control);

    bool groupIsEnabled();
    bool bandIsEnabled(Eq::Control enabled_control);

    void addParameterListeners();
    void removeParameterListeners();

private:
    FilterDsp::MonoChain& filter_chain_;
    double                sample_rate_;
    std::vector< double > magnitudes_;

    juce::Atomic< bool > should_repaint_;

    juce::AudioProcessorValueTreeState& apvts_;

    enum NODE_POS { LF, LMF, HMF, HF };
    static const int                                         NUM_NODES     = 4;
    static const int                                         NODE_DIAMETER = 8;
    static const int                                         NODE_RADIUS   = 4;
    typedef std::pair< juce::Value, juce::Value >            NodeXY;
    std::array< std::unique_ptr< AnalyserNode >, NUM_NODES > nodes_;
    std::array< NodeXY, NUM_NODES >                          node_coordinates_;
};
