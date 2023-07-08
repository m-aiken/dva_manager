#pragma once

#include "JuceHeader.h"

//==============================================================================
class EnablementButton : public juce::ToggleButton
{
public:
    EnablementButton(const juce::String& button_text, juce::RangedAudioParameter* param);

    void paint(juce::Graphics& g) override;

private:
    juce::ButtonParameterAttachment param_attachment_;
};
