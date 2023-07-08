#pragma once

#include <JuceHeader.h>

#include "CustomGridCell.h"
#include "../FolderView/FolderButton.h"
#include "../PluginProcessor.h"
#include "../Widgets/FileButton.h"

//==============================================================================
class FileInfo : public juce::Component
{
public:
    FileInfo(DvaManagerProcessor& processor_ref);

    void paint(juce::Graphics& g) override;
    void paintOverChildren(juce::Graphics& g) override;
    void resized() override;

    void setData(juce::ValueTree& dva_data);
    void updateGridCells();
    void setGridCell(CustomGridCell& cell, const juce::String& new_value);
    void setDvaNumber(const juce::String& dva_num);

private:
    void openButtonClicked();
    void updateFileSelection(juce::File& file);

    juce::Label dva_num_label_;
    FileButton  open_button_;

    CustomGridCell file_name_label_;
    CustomGridCell file_name_value_;
    juce::Label    file_path_value_;
    CustomGridCell file_size_label_;
    CustomGridCell file_size_value_;
    CustomGridCell duration_label_;
    CustomGridCell duration_value_;
    CustomGridCell num_channels_label_;
    CustomGridCell num_channels_value_;
    CustomGridCell sample_rate_label_;
    CustomGridCell sample_rate_value_;

    juce::AudioFormatManager                         format_manager_;
    std::unique_ptr< juce::AudioFormatReaderSource > reader_source_;
    std::unique_ptr< juce::FileChooser >             file_chooser_;

    juce::ValueTree dva_data_;

    DvaManagerProcessor& processor_ref_;
};
