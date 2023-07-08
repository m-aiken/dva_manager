#pragma once

#include <JuceHeader.h>

#include "ValueHolder.h"

//==============================================================================
class MeterValueLabel
    : public juce::Label
    , public juce::Timer
{
public:
    MeterValueLabel(std::function< float() >&& value_getter_fn);
    ~MeterValueLabel();

    void paint(juce::Graphics& g) override;

    void timerCallback() override;

private:
    std::function< float() > getValueFn;

    ValueHolder value_holder_;
};
