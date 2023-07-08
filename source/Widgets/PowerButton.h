#pragma once

#include <JuceHeader.h>

//==============================================================================
class PowerButton : public juce::ToggleButton
{
public:
    PowerButton(juce::RangedAudioParameter* param);

    void paint(juce::Graphics& g) override;

private:
    juce::ButtonParameterAttachment param_attachment_;
};
