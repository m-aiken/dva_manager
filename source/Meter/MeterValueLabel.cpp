#include "MeterValueLabel.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
MeterValueLabel::MeterValueLabel(std::function< float() >&& value_getter_fn)
    : getValueFn(std::move(value_getter_fn))
{
    startTimerHz(60);
}

/*---------------------------------------------------------------------------
**
*/
MeterValueLabel::~MeterValueLabel()
{
    stopTimer();
}

/*---------------------------------------------------------------------------
**
*/
void
MeterValueLabel::paint(juce::Graphics& g)
{
    float float_value       = value_holder_.getHeldValue();
    auto  background_colour = float_value > 0.f ? TailwindColours::Red400 : TailwindColours::Amber50;
    auto  text_colour       = float_value > 0.f ? TailwindColours::Amber50 : TailwindColours::Gray900;

    // Background.
    g.setColour(background_colour);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 2.f);

    // Text.
    g.setColour(text_colour);

    juce::String text_value;

    if (float_value <= -48.f) {
        text_value = "- " + juce::String::createStringFromData("∞", 16);
    }
    else {
        text_value = juce::String(float_value);
    }

    // Truncate the value to 2 decimal places.
    if (text_value.contains(".")) {
        text_value = text_value.substring(0, text_value.indexOf(".") + 3);
    }

    g.drawFittedText(text_value + " dB", getLocalBounds(), juce::Justification::centred, 1);
}

/*---------------------------------------------------------------------------
**
*/
void
MeterValueLabel::timerCallback()
{
    value_holder_.setHeldValue(getValueFn());
    repaint();
}

/*---------------------------------------------------------------------------
** End of File
*/
