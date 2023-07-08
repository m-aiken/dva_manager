#pragma once

#include <JuceHeader.h>

#include "Canvas.h"
#include "FFTBuffer.h"
#include "ResponseCurve.h"
#include "Spectrogram.h"
#include "../PluginProcessor.h"

//==============================================================================
class Analyser : public juce::Component
{
public:
    Analyser(DvaManagerProcessor& processor_ref);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    Canvas        canvas_;
    Spectrogram   spectrogram_;
    ResponseCurve response_curve_;
};
