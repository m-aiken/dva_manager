#pragma once

#include <JuceHeader.h>

//==============================================================================
class ValueHolder : public juce::Timer
{
public:
    ValueHolder();
    ~ValueHolder();

    void timerCallback() override;

    void  setHeldValue(float new_value);
    float getHeldValue();

private:
    float       held_value_;
    juce::int64 hold_time_ms_;
    juce::int64 last_update_time_ms_;
};
