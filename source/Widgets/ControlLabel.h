#pragma once

#include "JuceHeader.h"

//==============================================================================
class ControlLabel : public juce::Label
{
public:
    ControlLabel(const juce::String& text)
        : juce::Label(text, text)
    {
        juce::Font local_font(14.f, juce::Font::bold);
        setFont(local_font);
        setJustificationType(juce::Justification::centred);
    }
};
