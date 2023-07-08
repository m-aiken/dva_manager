#include "CompRotary.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
CompRotary::CompRotary(juce::AudioProcessorValueTreeState& apvts, RotaryTypes::Type rotary_type, Comp::Control comp_control)
    : RotaryBase(apvts.getParameter(Comp::str(comp_control)), rotary_type)
    , slider_attachment_(apvts, Comp::str(comp_control), *this)
    , group_enabled_(dynamic_cast< juce::AudioParameterBool* >(apvts.getParameter(Comp::str(Comp::ENABLED))))
    , group_enabled_raw_(apvts.getParameter(Comp::str(Comp::ENABLED)))
{
    setTitle(Comp::str(comp_control));

    group_enabled_raw_->addListener(this);
}

/*---------------------------------------------------------------------------
**
*/
CompRotary::~CompRotary()
{
    if (group_enabled_raw_ != nullptr) {
        group_enabled_raw_->removeListener(this);
    }
}

/*---------------------------------------------------------------------------
**
*/
bool
CompRotary::isEnabled()
{
    if (group_enabled_ != nullptr) {
        return group_enabled_->get();
    }

    return false;
}

/*---------------------------------------------------------------------------
** End of File
*/
