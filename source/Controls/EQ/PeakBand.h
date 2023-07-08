#pragma once

#include "JuceHeader.h"

#include "EqProperties.h"
#include "../../Widgets/ControlLabel.h"
#include "../../Widgets/EqRotary.h"
#include "../../Widgets/PowerButton.h"

//==============================================================================
class PeakBand : public juce::Component
{
public:
    PeakBand(const juce::String&                 label_text,
             juce::AudioProcessorValueTreeState& apvts,
             Eq::Control                         enabled_ctrl,
             Eq::Control                         freq_ctrl,
             Eq::Control                         gain_ctrl,
             Eq::Control                         q_ctrl);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ControlLabel label_;
    PowerButton  enabled_btn_;
    EqRotary     freq_;
    EqRotary     gain_;
    EqRotary     q_;
};
