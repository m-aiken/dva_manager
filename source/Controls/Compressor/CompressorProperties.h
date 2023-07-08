#pragma once

#include "JuceHeader.h"

//==============================================================================
namespace Comp
{

enum Control { ENABLED = 200, INPUT_GAIN, THRESHOLD, RATIO, ATTACK, RELEASE, MAKEUP_GAIN };

static const juce::String
str(Control c)
{
    return juce::String(c);
}

static const juce::NormalisableRange THRESHOLD_RANGE(-20.f, 20.f, 1.f);
static const juce::NormalisableRange GAIN_RANGE(-20.f, 20.f, 1.f);
static const juce::StringArray       RATIO_CHOICES("1.5", "2", "3", "4", "5", "10");
static const juce::StringArray       ATTACK_CHOICES(".1", ".3", "1", "3", "10", "30");
static const juce::StringArray       RELEASE_CHOICES(".1", ".2", ".4", ".8", "1.6", "auto");

}  // namespace Comp
