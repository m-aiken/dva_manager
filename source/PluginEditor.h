#pragma once

#include "PluginProcessor.h"
#include "FolderView/FolderSelector.h"
#include "MainView/MainView.h"
#include "Menu/DvaListBox.h"
#include "Utilities/GlobalLookAndFeel.h"
#include "Widgets/ViewButton.h"

//==============================================================================
class DvaManagerEditor : public juce::AudioProcessorEditor
{
public:
    explicit DvaManagerEditor(DvaManagerProcessor&);
    ~DvaManagerEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void handleViewSwitch();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DvaManagerProcessor& processorRef;

    DvaListBox     slot_browser_;
    MainView       main_view_;
    FolderSelector folder_view_;
    ViewButton     view_button_;

    GlobalLookAndFeel lnf_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DvaManagerEditor)
};
