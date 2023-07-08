#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace RotaryTypes
{

enum Type { FREQUENCY = 300, GAIN, RATIO, TIME, Q };

static const juce::String
suffix(Type t)
{
    switch (t) {
    case FREQUENCY:
        return "Hz";
    case GAIN:
        return "dB";
    case RATIO:
        return ":1";
    case TIME:
        return "ms";
    case Q:
        return "Q";
    default:
        return "";
    }
}

}  // namespace RotaryTypes
