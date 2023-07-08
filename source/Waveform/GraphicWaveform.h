#pragma once

#include "JuceHeader.h"

//==============================================================================
class GraphicWaveform
    : public juce::AudioThumbnail
    , public juce::Component
{
public:
    GraphicWaveform(int num_samples, juce::AudioFormatManager& format_manager, juce::AudioThumbnailCache& cache);

    void paint(juce::Graphics& g) override;
};
