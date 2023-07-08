#pragma once

#include "JuceHeader.h"

//==============================================================================
class DvaListBox
    : public juce::ListBoxModel
    , public juce::Component
{
public:
    DvaListBox(juce::ValueTree& tree_state);
    ~DvaListBox();

    void resized() override;

    int  getNumRows() override;
    void paintListBoxItem(int row_number, Graphics& g, int width, int height, bool row_is_selected) override;
    void selectedRowsChanged(int last_row_selected) override;

private:
    juce::ListBox list_box_;

    juce::ValueTree& tree_state_;
};
