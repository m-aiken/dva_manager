#pragma once

#include "JuceHeader.h"

#include "../Controls/EQ/EqProperties.h"

//==============================================================================
class AnalyserNode : public juce::ToggleButton
{
public:
    AnalyserNode(juce::AudioProcessorValueTreeState& apvts, Eq::Control band_control);

    void paint(juce::Graphics& g) override;

private:
    juce::AudioProcessorValueTreeState::ButtonAttachment param_attachment_;
};
