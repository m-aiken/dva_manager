#include "PowerButton.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
PowerButton::PowerButton(juce::RangedAudioParameter* param)
    : juce::ToggleButton()
    , param_attachment_(*param, *this, nullptr)
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

/*---------------------------------------------------------------------------
**
*/
void
PowerButton::paint(juce::Graphics& g)
{
    auto bounds        = getLocalBounds().toFloat();
    auto on            = getToggleState();
    auto button_colour = on ? TailwindColours::Emerald300 : TailwindColours::Red400;
    auto orb_padding   = 1;
    auto orb_diameter  = bounds.getHeight() - (orb_padding * 2);
    auto orb_x         = on ? bounds.getWidth() - orb_diameter - orb_padding : orb_padding;

    // Background.
    g.setColour(button_colour);
    g.fillRoundedRectangle(bounds.toFloat(), 8.f);

    // Orb.
    g.setColour(TailwindColours::Amber50);
    g.fillEllipse(orb_x, orb_padding, orb_diameter, orb_diameter);
}

/*---------------------------------------------------------------------------
** End of File
*/
