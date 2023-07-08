#pragma once

#include <JuceHeader.h>

//==============================================================================
class Canvas : public juce::Component
{
public:
    Canvas();

    void paint(juce::Graphics& g) override;
    void resized() override;

    bool frequencyToDraw(int freq);
    bool shouldBeBoldLine(int freq);

private:
    juce::ColourGradient background_gradient_;
};
