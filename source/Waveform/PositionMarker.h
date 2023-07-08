#pragma once

#include "JuceHeader.h"

#include "GraphicWaveform.h"

//==============================================================================
class PositionMarker
    : public juce::Component
    , public juce::Timer
{
public:
    PositionMarker(GraphicWaveform& waveform, juce::AudioTransportSource& transport_source);
    ~PositionMarker();

    void paint(juce::Graphics& g) override;

    void timerCallback() override;

private:
    GraphicWaveform&            waveform_;
    juce::AudioTransportSource& transport_source_;
};
