#pragma once

#include "JuceHeader.h"

#include "RotaryBase.h"
#include "RotaryTypes.h"
#include "../Controls/EQ/EqProperties.h"

//==============================================================================
class EqRotary : public RotaryBase
{
public:
    EqRotary(juce::AudioProcessorValueTreeState& apvts,
             RotaryTypes::Type                   rotary_type,
             Eq::Control                         eq_control,
             Eq::Control                         param_enabled_control);
    ~EqRotary();

    bool isEnabled() override;

private:
    juce::AudioProcessorValueTreeState::SliderAttachment slider_attachment_;

    juce::AudioParameterBool*   group_enabled_;
    juce::RangedAudioParameter* group_enabled_raw_;
    juce::AudioParameterBool*   param_enabled_;
    juce::RangedAudioParameter* param_enabled_raw_;
};
