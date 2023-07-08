#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace Global
{

static const float MAX_DB  = 12.f;
static const float NEG_INF = -48.f;

static const float MIN_HZ = 20.f;
static const float MAX_HZ = 20000.f;

enum RadioGroup {
    LC_SLOPE_GROUP       = 1001,
    HC_SLOPE_GROUP       = 2001,
    EQ_COMP_SELECT_GROUP = 3001,
    METER_SELECT_GROUP   = 4001
};

// The bounds for the decibel markers needs to be taller than the drawable area.
// This is to allow for the correct centering of the text at each y coordinate.
static const int DB_Y_OVERFLOW_PX = 48;

static const int ANALYSER_HEIGHT = 360;

enum View { MAIN_VIEW, FOLDER_VIEW };

static const float BUTTON_CORNER_RADIUS = 3.f;

}  // namespace Global
