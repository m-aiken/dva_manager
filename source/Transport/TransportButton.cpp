#include "TransportButton.h"
#include "../Utilities/GlobalConstants.h"

/*---------------------------------------------------------------------------
**
*/
TransportButton::TransportButton(const juce::String& name,
                                 const juce::Colour& standard_colour,
                                 const juce::Colour& hover_colour)
    : juce::DrawableButton(name, juce::DrawableButton::ImageFitted)
    , standard_colour_(standard_colour)
    , hover_colour_(hover_colour)
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    setColour(juce::DrawableButton::backgroundColourId, standard_colour_);
}

/*---------------------------------------------------------------------------
**
*/
void
TransportButton::paintButton(juce::Graphics& g, bool should_draw_button_as_highlighted, bool should_draw_button_as_down)
{
    juce::ignoreUnused(should_draw_button_as_highlighted, should_draw_button_as_down);

    g.setColour(findColour(juce::DrawableButton::backgroundColourId));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), Global::BUTTON_CORNER_RADIUS);
}

/*---------------------------------------------------------------------------
**
*/
void
TransportButton::mouseEnter(const juce::MouseEvent& event)
{
    juce::ignoreUnused(event);

    setColour(juce::DrawableButton::backgroundColourId, hover_colour_);
}

/*---------------------------------------------------------------------------
**
*/
void
TransportButton::mouseExit(const juce::MouseEvent& event)
{
    juce::ignoreUnused(event);

    setColour(juce::DrawableButton::backgroundColourId, standard_colour_);
}

/*---------------------------------------------------------------------------
** End of File
*/
