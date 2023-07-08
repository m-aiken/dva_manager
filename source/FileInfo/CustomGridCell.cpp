#include "CustomGridCell.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
CustomGridCell::CustomGridCell(CELL_TYPE cell_type, const juce::String& component_name, const juce::String& label_text)
    : juce::Label(component_name, label_text)
    , cell_type_(cell_type)
{
}

/*---------------------------------------------------------------------------
**
*/
void
CustomGridCell::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Background colour.
    g.fillAll((cell_type_ == HEADER) ? TailwindColours::Gray900.withAlpha(0.2f) : TailwindColours::Gray800);

    // Text.
    g.setColour(TailwindColours::Amber50);

    juce::Font local_font(14.f, (cell_type_ == HEADER) ? juce::Font::bold : juce::Font::plain);
    g.setFont(local_font);

    g.drawFittedText(getText(), 0, 0, bounds.getWidth(), bounds.getHeight(), juce::Justification::centred, 1);
}

/*---------------------------------------------------------------------------
** End of File
*/
