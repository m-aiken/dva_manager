#pragma once

#include "JuceHeader.h"

//==============================================================================
class GlobalLookAndFeel : public juce::LookAndFeel_V4
{
public:
    GlobalLookAndFeel();

    void drawRotarySlider(juce::Graphics& g,
                          int             x,
                          int             y,
                          int             width,
                          int             height,
                          float           slider_pos_proportional,
                          float           rotary_start_angle,
                          float           rotary_end_angle,
                          juce::Slider&   slider) override;

    void drawToggleButton(juce::Graphics&     g,
                          juce::ToggleButton& button,
                          bool                should_draw_button_as_highlighted,
                          bool                should_draw_button_as_down) override;
};
