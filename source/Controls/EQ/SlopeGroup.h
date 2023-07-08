#pragma once

#include "JuceHeader.h"

#include "../../Utilities/GlobalConstants.h"
#include "../../Widgets/RadioButton.h"

//==============================================================================
class SlopeGroup : public juce::Component
{
public:
    SlopeGroup(juce::RangedAudioParameter* param, Global::RadioGroup radio_group);

    void resized() override;

    void setSlope(juce::ToggleButton* button, uint8_t new_index);

private:
    juce::RangedAudioParameter* param_;

    RadioButton slope_12_button_;
    RadioButton slope_24_button_;
    RadioButton slope_36_button_;
    RadioButton slope_48_button_;
};
