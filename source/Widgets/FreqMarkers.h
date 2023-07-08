#pragma once

#include "JuceHeader.h"

//==============================================================================
class FreqMarkers : public juce::Component
{
public:
    FreqMarkers();

    void paint(juce::Graphics& g) override;

    bool inCatchGroup(int freq);
};
