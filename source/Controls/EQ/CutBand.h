#pragma once

#include "JuceHeader.h"

#include "EqProperties.h"
#include "SlopeGroup.h"
#include "../../Utilities/GlobalConstants.h"
#include "../../Widgets/ControlLabel.h"
#include "../../Widgets/EqRotary.h"
#include "../../Widgets/PowerButton.h"

//==============================================================================
class CutBand : public juce::Component
{
public:
    CutBand(const juce::String&                 label_text,
            juce::AudioProcessorValueTreeState& apvts,
            Eq::Control                         enabled_ctrl,
            Eq::Control                         freq_ctrl,
            Eq::Control                         slope_ctrl,
            Global::RadioGroup                  radio_group);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ControlLabel label_;
    PowerButton  enabled_btn_;
    EqRotary     freq_;
    SlopeGroup   slope_radios_;
    Eq::Control  slope_ctrl_;
};
