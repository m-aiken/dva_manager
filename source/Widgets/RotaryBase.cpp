#include "RotaryBase.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
RotaryBase::RotaryBase(juce::RangedAudioParameter* parameter, RotaryTypes::Type rotary_type)
    : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
    , param_(parameter)
    , rotary_type_(rotary_type)
{
    setMouseCursor(juce::MouseCursor::UpDownLeftRightResizeCursor);
}

/*---------------------------------------------------------------------------
**
*/
RotaryBase::~RotaryBase()
{
}

/*---------------------------------------------------------------------------
**
*/
void
RotaryBase::paint(juce::Graphics& g)
{
    if (param_ == nullptr) {
        return;
    }

    setEnabled(isEnabled());

    auto slider_bounds = getRotaryBounds();

    getLookAndFeel().drawRotarySlider(g,
                                      slider_bounds.getX(),
                                      slider_bounds.getY(),
                                      slider_bounds.getWidth(),
                                      slider_bounds.getHeight(),
                                      getValueToDraw(),
                                      start_angle_,
                                      end_angle_,
                                      *this);

    // Value text.
    g.setColour(isEnabled() ? TailwindColours::Amber50 : TailwindColours::Gray600);
    g.setFont(font_);

    juce::String value_str;
    juce::String value_suffix = RotaryTypes::suffix(rotary_type_);

    if (rotary_type_ != RotaryTypes::Q) {
        if (rotary_type_ == RotaryTypes::FREQUENCY && param_->convertFrom0to1(param_->getValue()) > 1000.f) {
            juce::String kHz_value = juce::String(param_->convertFrom0to1(param_->getValue()) / 1000.f);
            value_str += kHz_value.substring(0, kHz_value.indexOf(".") + 2);
            value_suffix = "k" + value_suffix;
        }
        else {
            value_str += param_->getCurrentValueAsText();
        }
    }

    value_str += value_suffix;

    g.drawFittedText(value_str, slider_bounds, juce::Justification::centred, 1);
}

/*---------------------------------------------------------------------------
**
*/
void
RotaryBase::parameterValueChanged(int parameter_index, float new_value)
{
    juce::ignoreUnused(parameter_index, new_value);

    repaint();
}

/*---------------------------------------------------------------------------
**
*/
void
RotaryBase::parameterGestureChanged(int parameter_index, bool gesture_is_starting)
{
    // Only implemented because it's a pure virtual method of juce::AudioProcessorParameter::Listener.
    juce::ignoreUnused(parameter_index, gesture_is_starting);
}

/*---------------------------------------------------------------------------
**
*/
float
RotaryBase::getValueToDraw()
{
    if (param_ != nullptr) {
        auto range = param_->getNormalisableRange();

        return juce::jmap< float >(getValue(), range.start, range.end, start_angle_, end_angle_);
    }

    return 0.f;
}

/*---------------------------------------------------------------------------
**
*/
juce::Rectangle< int >
RotaryBase::getRotaryBounds() const
{
    auto bounds = getLocalBounds();
    auto size   = juce::jmin(bounds.getWidth(), bounds.getHeight());

    //    size -= bounds.getHeight() * 0.1;

    juce::Rectangle< int > r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), bounds.getCentreY());

    return r;
}

/*---------------------------------------------------------------------------
** End of File
*/
