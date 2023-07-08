#pragma once

#include "JuceHeader.h"

//==============================================================================
class TransportButton : public juce::DrawableButton
{
public:
    TransportButton(const juce::String& name, const juce::Colour& standard_colour, const juce::Colour& hover_colour);

    void paintButton(juce::Graphics& g, bool should_draw_button_as_highlighted, bool should_draw_button_as_down) override;

    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;

private:
    juce::Colour standard_colour_;
    juce::Colour hover_colour_;
};
