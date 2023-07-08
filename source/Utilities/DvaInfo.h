#pragma once

#include "JuceHeader.h"

//==============================================================================
namespace DvaInfo
{

enum DVA_FIELD { DVA_NUM, FILENAME, PATH, SIZE, DURATION_SECONDS, NUM_CHANNELS, SAMPLE_RATE, PEAK, RMS };

static const juce::String
getXmlProperty(DVA_FIELD dva_field)
{
    switch (dva_field) {
    case DVA_NUM:
        return "slot_num";
    case FILENAME:
        return "file_name";
    case PATH:
        return "file_location";
    case SIZE:
        return "size_mb";
    case DURATION_SECONDS:
        return "duration_seconds";
    case NUM_CHANNELS:
        return "num_channels";
    case SAMPLE_RATE:
        return "sample_rate_hz";
    default:
        return "";
    }
}

}  // namespace DvaInfo
