#pragma once

#include "JuceHeader.h"

#include "CutBand.h"
#include "PeakBand.h"

//==============================================================================
class EqControls : public juce::Component
{
public:
    EqControls(juce::AudioProcessorValueTreeState& apvts);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    CutBand  low_cut_;
    PeakBand lf_band_;
    PeakBand lmf_band_;
    PeakBand hmf_band_;
    PeakBand hf_band_;
    CutBand  high_cut_;
};
