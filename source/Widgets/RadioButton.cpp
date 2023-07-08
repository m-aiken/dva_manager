#include "RadioButton.h"

/*---------------------------------------------------------------------------
**
*/
RadioButton::RadioButton(const juce::String& text)
    : juce::ToggleButton(text)
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

/*---------------------------------------------------------------------------
**
*/
void
RadioButton::paint(juce::Graphics& g)
{
    getLookAndFeel().drawToggleButton(g, *this, false, false);
}

/*---------------------------------------------------------------------------
** End of File
*/
