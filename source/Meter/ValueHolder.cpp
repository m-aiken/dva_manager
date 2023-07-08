#include "ValueHolder.h"
#include "../Utilities/GlobalConstants.h"

/*---------------------------------------------------------------------------
**
*/
ValueHolder::ValueHolder()
    : held_value_(Global::NEG_INF)
    , hold_time_ms_(2000)
    , last_update_time_ms_(juce::Time::currentTimeMillis())
{
    startTimerHz(60);
}

/*---------------------------------------------------------------------------
**
*/
ValueHolder::~ValueHolder()
{
    stopTimer();
}

/*---------------------------------------------------------------------------
**
*/
void
ValueHolder::timerCallback()
{
    if ((juce::Time::currentTimeMillis() - last_update_time_ms_) > hold_time_ms_) {
        held_value_          = Global::NEG_INF;
        last_update_time_ms_ = juce::Time::currentTimeMillis();
    }
}

/*---------------------------------------------------------------------------
**
*/
void
ValueHolder::setHeldValue(float new_value)
{
    if (new_value > held_value_) {
        held_value_          = new_value;
        last_update_time_ms_ = juce::Time::currentTimeMillis();
    }
}

/*---------------------------------------------------------------------------
**
*/
float
ValueHolder::getHeldValue()
{
    return held_value_;
}

/*---------------------------------------------------------------------------
** End of File
*/
