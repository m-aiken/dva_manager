#pragma once

#include "JuceHeader.h"

#include "RotaryTypes.h"
#include "../Controls/EQ/EqProperties.h"
#include "../Controls/Compressor/CompressorProperties.h"

//==============================================================================
class RotaryBase
    : public juce::Slider
    , public juce::AudioProcessorParameter::Listener
{
public:
    RotaryBase(juce::RangedAudioParameter* parameter, RotaryTypes::Type rotary_type);
    ~RotaryBase();

    void paint(juce::Graphics& g) override;

    void parameterValueChanged(int parameter_index, float new_value) override;
    void parameterGestureChanged(int parameter_index, bool gesture_is_starting) override;

    float                                getValueToDraw();
    [[nodiscard]] juce::Rectangle< int > getRotaryBounds() const;

    virtual bool isEnabled() = 0;

private:
    juce::RangedAudioParameter* param_;
    RotaryTypes::Type           rotary_type_;
    //    juce::AudioProcessorValueTreeState::SliderAttachment slider_attachment_;

    //    juce::AudioParameterBool*   group_enabled_;
    //    juce::RangedAudioParameter* group_enabled_raw_;

    static constexpr float start_angle_ = juce::degreesToRadians(180.f + 45.f);
    static constexpr float end_angle_   = juce::degreesToRadians(180.f - 45.f) + juce::MathConstants< float >::twoPi;
    const juce::Font       font_ { 12.f, juce::Font::plain };
};
