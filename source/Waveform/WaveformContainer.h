#pragma once

#include <JuceHeader.h>

#include "GraphicWaveform.h"
#include "PositionMarker.h"
#include "../PluginProcessor.h"

//==============================================================================
class WaveformContainer : public juce::Component
{
public:
    WaveformContainer(DvaManagerProcessor& processor_ref);

    void resized() override;

    void setData(juce::ValueTree& dva_data);
    void updateWaveform();

private:
    juce::AudioThumbnailCache waveform_cache_;
    GraphicWaveform           waveform_;
    PositionMarker            position_marker_;

    juce::ValueTree dva_data_;
};
