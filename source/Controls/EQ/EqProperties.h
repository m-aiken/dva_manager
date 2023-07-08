#pragma once

#include "JuceHeader.h"

#include "../../Utilities/GlobalConstants.h"

//==============================================================================
namespace Eq
{

enum Control {
    ENABLED = 100,
    /*-------------------------*/
    LOW_CUT_FREQ,
    LOW_CUT_SLOPE,
    LOW_CUT_ENABLED,
    /*-------------------------*/
    LF_FREQ,
    LF_GAIN,
    LF_QUALITY,
    LF_ENABLED,
    /*-------------------------*/
    LMF_FREQ,
    LMF_GAIN,
    LMF_QUALITY,
    LMF_ENABLED,
    /*-------------------------*/
    HMF_FREQ,
    HMF_GAIN,
    HMF_QUALITY,
    HMF_ENABLED,
    /*-------------------------*/
    HF_FREQ,
    HF_GAIN,
    HF_QUALITY,
    HF_ENABLED,
    /*-------------------------*/
    HIGH_CUT_FREQ,
    HIGH_CUT_SLOPE,
    HIGH_CUT_ENABLED,
    /*-------------------------*/
    FIRST_PARAM = ENABLED,
    LAST_PARAM  = HIGH_CUT_ENABLED,
    NUM_PARAMS  = LAST_PARAM - FIRST_PARAM + 1
};

static const juce::String
str(Control c)
{
    return juce::String(c);
}

static const std::array< juce::String, NUM_PARAMS > PARAM_IDS = {
    str(ENABLED),
    /*-------------------------*/
    str(LOW_CUT_FREQ),
    str(LOW_CUT_SLOPE),
    str(LOW_CUT_ENABLED),
    /*-------------------------*/
    str(LF_FREQ),
    str(LF_GAIN),
    str(LF_QUALITY),
    str(LF_ENABLED),
    /*-------------------------*/
    str(LMF_FREQ),
    str(LMF_GAIN),
    str(LMF_QUALITY),
    str(LMF_ENABLED),
    /*-------------------------*/
    str(HMF_FREQ),
    str(HMF_GAIN),
    str(HMF_QUALITY),
    str(HMF_ENABLED),
    /*-------------------------*/
    str(HF_FREQ),
    str(HF_GAIN),
    str(HF_QUALITY),
    str(HF_ENABLED),
    /*-------------------------*/
    str(HIGH_CUT_FREQ),
    str(HIGH_CUT_SLOPE),
    str(HIGH_CUT_ENABLED)
    /*-------------------------*/
};

static const juce::NormalisableRange LOW_CUT_FREQ_RANGE(Global::MIN_HZ, 3000.f, 1.f, 1.f);
static const juce::NormalisableRange HIGH_CUT_FREQ_RANGE(300.f, Global::MAX_HZ, 1.f, 1.f);
static const juce::StringArray       SLOPE_CHOICES("12 dB/Oct", "24 dB/Oct", "36 dB/Oct", "48 dB/Oct");

static const juce::NormalisableRange PEAK_GAIN_RANGE(Global::NEG_INF, Global::MAX_DB, 0.5f, 1.f);
static const juce::NormalisableRange PEAK_Q_RANGE(0.1f, 10.f, 0.05f, 1.f);

static const juce::NormalisableRange LF_FREQ_RANGE(30.f, 450.f, 1.f, 1.f);
static const juce::NormalisableRange LMF_FREQ_RANGE(200.f, 2500.f, 1.f, 1.f);
static const juce::NormalisableRange HMF_FREQ_RANGE(600.f, 7000.f, 1.f, 1.f);
static const juce::NormalisableRange HF_FREQ_RANGE(1500.f, 16000.f, 1.f, 1.f);

}  // namespace Eq
