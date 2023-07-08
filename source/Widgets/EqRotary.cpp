#include "EqRotary.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
EqRotary::EqRotary(juce::AudioProcessorValueTreeState& apvts,
                   RotaryTypes::Type                   rotary_type,
                   Eq::Control                         eq_control,
                   Eq::Control                         param_enabled_control)
    : RotaryBase(apvts.getParameter(Eq::str(eq_control)), rotary_type)
    , slider_attachment_(apvts, Eq::str(eq_control), *this)
    , group_enabled_(dynamic_cast< juce::AudioParameterBool* >(apvts.getParameter(Eq::str(Eq::ENABLED))))
    , group_enabled_raw_(apvts.getParameter(Eq::str(Eq::ENABLED)))
    , param_enabled_(dynamic_cast< juce::AudioParameterBool* >(apvts.getParameter(Eq::str(param_enabled_control))))
    , param_enabled_raw_(apvts.getParameter(Eq::str(param_enabled_control)))
{
    setTitle(Eq::str(eq_control));

    group_enabled_raw_->addListener(this);
    param_enabled_raw_->addListener(this);
}

/*---------------------------------------------------------------------------
**
*/
EqRotary::~EqRotary()
{
    if (group_enabled_raw_ != nullptr) {
        group_enabled_raw_->removeListener(this);
    }

    if (param_enabled_raw_ != nullptr) {
        param_enabled_raw_->removeListener(this);
    }
}

/*---------------------------------------------------------------------------
**
*/
bool
EqRotary::isEnabled()
{
    if (group_enabled_ != nullptr && param_enabled_ != nullptr) {
        return (group_enabled_->get() && param_enabled_->get());
    }

    return false;
}

/*---------------------------------------------------------------------------
** End of File
*/
