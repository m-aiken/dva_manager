#include "SlopeGroup.h"
#include "../../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
SlopeGroup::SlopeGroup(juce::RangedAudioParameter* param, Global::RadioGroup radio_group)
    : param_(param)
    , slope_12_button_("12")
    , slope_24_button_("24")
    , slope_36_button_("36")
    , slope_48_button_("48")
{
    addAndMakeVisible(&slope_12_button_);
    addAndMakeVisible(&slope_24_button_);
    addAndMakeVisible(&slope_36_button_);
    addAndMakeVisible(&slope_48_button_);

    slope_12_button_.setRadioGroupId(radio_group);
    slope_24_button_.setRadioGroupId(radio_group);
    slope_36_button_.setRadioGroupId(radio_group);
    slope_48_button_.setRadioGroupId(radio_group);

    slope_12_button_.onClick = [&]() { setSlope(&slope_12_button_, 0); };
    slope_24_button_.onClick = [&]() { setSlope(&slope_24_button_, 1); };
    slope_36_button_.onClick = [&]() { setSlope(&slope_36_button_, 2); };
    slope_48_button_.onClick = [&]() { setSlope(&slope_48_button_, 3); };

    slope_12_button_.setToggleState(true, juce::NotificationType::sendNotification);
}

/*---------------------------------------------------------------------------
**
*/
void
SlopeGroup::resized()
{
    using Tr = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    juce::Grid grid;

    grid.autoColumns  = Tr(Fr(1));
    grid.templateRows = { Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)) };

    grid.items.add(juce::GridItem(slope_12_button_));
    grid.items.add(juce::GridItem(slope_24_button_));
    grid.items.add(juce::GridItem(slope_36_button_));
    grid.items.add(juce::GridItem(slope_48_button_));

    grid.setGap(juce::Grid::Px { 4 });

    grid.performLayout(getLocalBounds());
}

/*---------------------------------------------------------------------------
**
*/
void
SlopeGroup::setSlope(juce::ToggleButton* button, uint8_t new_index)
{
    if ((param_ == nullptr) || (button == nullptr) || !button->getToggleState()) {
        return;
    }

    auto choice_param = dynamic_cast< juce::AudioParameterChoice* >(param_);

    if (choice_param != nullptr) {
        *choice_param = new_index;
    }
}

/*---------------------------------------------------------------------------
** End of File
*/
