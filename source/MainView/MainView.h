#pragma once

#include "JuceHeader.h"

#include "../PluginProcessor.h"
#include "../Analyser/Analyser.h"
#include "../Controls/ControlsContainer.h"
#include "../FileInfo/FileInfo.h"
#include "../Meter/MeterGroup.h"
#include "../Transport/TransportBar.h"
#include "../Waveform/WaveformContainer.h"
#include "../Widgets/DbMarkers.h"
#include "../Widgets/FreqMarkers.h"

//==============================================================================
class MainView
    : public juce::Component
    , public juce::ValueTree::Listener
    , public juce::FileDragAndDropTarget
{
public:
    MainView(DvaManagerProcessor& p);
    ~MainView();

    void paint(juce::Graphics& g) override;
    void resized() override;

    void valueTreePropertyChanged(juce::ValueTree& value_tree, const juce::Identifier& property) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void setSelectedDva(const juce::String& selected_dva);
    void updateFileSelection(juce::File& file);

private:
    DvaManagerProcessor& processor_ref_;
    juce::ValueTree      dva_data_;

    FileInfo          file_info_;
    Analyser          analyser_;
    FreqMarkers       analyser_freq_markers_;
    DbMarkers         analyser_db_markers_;
    ControlsContainer controls_container_;
    MeterGroup        meter_group_;
    WaveformContainer waveform_;
    TransportBar      transport_bar_;
};
