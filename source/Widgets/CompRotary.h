#pragma once

#include "JuceHeader.h"

#include "RotaryBase.h"
#include "RotaryTypes.h"
#include "../Controls/Compressor/CompressorProperties.h"

//==============================================================================
class CompRotary : public RotaryBase
{
public:
    CompRotary(juce::AudioProcessorValueTreeState& apvts, RotaryTypes::Type rotary_type, Comp::Control comp_control);
    ~CompRotary();

    bool isEnabled() override;

private:
    juce::AudioProcessorValueTreeState::SliderAttachment slider_attachment_;

    juce::AudioParameterBool*   group_enabled_;
    juce::RangedAudioParameter* group_enabled_raw_;
};
