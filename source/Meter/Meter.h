#pragma once

#include <JuceHeader.h>

#include "../PluginProcessor.h"
#include "../Utilities/GlobalConstants.h"

//==============================================================================
class Meter
    : public juce::Component
    , public juce::Timer
{
public:
    Meter(std::function< float() >&& value_getter_fn);
    ~Meter();

    void paint(juce::Graphics& g) override;
    void resized() override;

    void timerCallback() override;

private:
    std::function< float() > getValueFn;
    juce::ColourGradient     colour_gradient_;
};
