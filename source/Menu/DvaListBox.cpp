#include "DvaListBox.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
DvaListBox::DvaListBox(juce::ValueTree& tree_state)
    : tree_state_(tree_state)
{
    addAndMakeVisible(&list_box_);

    list_box_.setRowHeight(24);
    list_box_.setModel(this);
    list_box_.getVerticalScrollBar().setColour(juce::ScrollBar::thumbColourId, TailwindColours::Emerald300);

    setSize(getLocalBounds().getWidth(), getLocalBounds().getHeight());

    // TODO is this the best place to initialise the list to row 1??
    list_box_.selectRow(0);
}

/*---------------------------------------------------------------------------
**
*/
DvaListBox::~DvaListBox()
{
}

/*---------------------------------------------------------------------------
**
*/
void
DvaListBox::resized()
{
    list_box_.setBounds(getLocalBounds());
}

/*---------------------------------------------------------------------------
**
*/
int
DvaListBox::getNumRows()
{
    return 64;
}

/*---------------------------------------------------------------------------
**
*/
void
DvaListBox::paintListBoxItem(int row_number, Graphics& g, int width, int height, bool row_is_selected)
{
    // Divider lines (top and right).
    g.setColour(TailwindColours::Amber50.withAlpha(0.25f));
    g.drawLine(0, 0, width, 0, 1.f);
    g.drawLine(width, 0, width, height, 1.f);

    // Selected row background.
    if (row_is_selected) {
        g.fillAll(TailwindColours::Emerald300);
    }

    // Text.
    g.setColour(row_is_selected ? TailwindColours::Gray800 : TailwindColours::Amber50);
    g.drawText("DVA " + juce::String(row_number + 1), 10, 0, width, height, juce::Justification::centredLeft, true);
}

/*---------------------------------------------------------------------------
**
*/
void
DvaListBox::selectedRowsChanged(int last_row_selected)
{
    if (tree_state_.isValid() && tree_state_.hasProperty("selected_dva")) {
        tree_state_.setProperty("selected_dva", juce::String(last_row_selected + 1), nullptr);
    }
}

/*---------------------------------------------------------------------------
** End of File
*/
