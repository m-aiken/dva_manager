#pragma once

#include "JuceHeader.h"

#include "../../Widgets/CompRotary.h"
#include "../../Widgets/ControlLabel.h"

//==============================================================================
class CompressorControls : public juce::Component
{
public:
    CompressorControls(juce::AudioProcessorValueTreeState& apvts);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    CompRotary input_gain_ctrl_;
    CompRotary threshold_ctrl_;
    CompRotary ratio_ctrl_;
    CompRotary attack_ctrl_;
    CompRotary release_ctrl_;
    CompRotary makeup_gain_ctrl_;

    ControlLabel input_gain_label_;
    ControlLabel threshold_label_;
    ControlLabel ratio_label_;
    ControlLabel attack_label_;
    ControlLabel release_label_;
    ControlLabel makeup_gain_label_;
};
