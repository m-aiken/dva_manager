#pragma once

#include <JuceHeader.h>

#include "FolderButton.h"
#include "../PluginProcessor.h"

//==============================================================================
class FolderSelector
    : public juce::Component
    , public juce::FileDragAndDropTarget
{
public:
    FolderSelector(DvaManagerProcessor& processor_ref);

    void paint(juce::Graphics& g) override;
    void resized() override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    juce::String status_label_;

private:
    void openButtonClicked();
    void updateState(const juce::String& dva_name, const juce::String& file_path);

    FolderButton folder_button_;

    std::unique_ptr< juce::FileChooser > file_chooser_;

    DvaManagerProcessor& processor_ref_;
};
