#pragma once

#include "JuceHeader.h"

//==============================================================================
class RadioButton : public juce::ToggleButton
{
public:
    RadioButton(const juce::String& text);

    void paint(juce::Graphics& g) override;
};
