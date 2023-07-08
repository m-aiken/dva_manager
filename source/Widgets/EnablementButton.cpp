#include "EnablementButton.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
EnablementButton::EnablementButton(const juce::String& button_text, juce::RangedAudioParameter* param)
    : juce::ToggleButton(button_text)
    , param_attachment_(*param, *this, nullptr)
{
}

/*---------------------------------------------------------------------------
**
*/
void
EnablementButton::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    auto fill_colour = TailwindColours::Gray800;
    auto orb_colour  = TailwindColours::Gray900;
    auto text_colour = TailwindColours::Amber50;
    auto status_text = "Disabled";

    if (getToggleState()) {
        fill_colour = TailwindColours::Blue400;
        orb_colour  = TailwindColours::Green400;
        status_text = "Enabled";
    }

    // Fill
    g.fillAll(fill_colour);

    // Orb.
    auto                     orb_diameter = 10;
    auto                     orb_radius   = orb_diameter * 0.5;
    juce::Rectangle< float > orb(orb_radius, bounds.getCentreY() - orb_radius, orb_diameter, orb_diameter);

    g.setColour(orb_colour);
    g.fillEllipse(orb);

    // Text.
    auto                   text_bounds_x     = orb_diameter;
    auto                   text_bounds_width = bounds.getRight() - text_bounds_x;
    juce::Rectangle< int > text_bounds(text_bounds_x, 0, text_bounds_width, bounds.getHeight());

    g.setColour(text_colour);
    g.drawFittedText(getButtonText() + " " + status_text, text_bounds, juce::Justification::centred, 1);
}

/*---------------------------------------------------------------------------
** End of File
*/
