#pragma once

#include <JuceHeader.h>

#include "Compressor/CompressorControls.h"
#include "EQ/EqControls.h"
#include "../Widgets/PowerButton.h"
#include "../Widgets/RadioButton.h"

//==============================================================================
class ControlsContainer : public juce::Component
{
public:
    ControlsContainer(juce::AudioProcessorValueTreeState& apvts);

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setView(juce::ToggleButton* button, uint8_t new_index);

private:
    PowerButton eq_enabled_btn_;
    PowerButton comp_enabled_btn_;

    RadioButton eq_select_btn_;
    RadioButton comp_select_btn_;

    EqControls         eq_controls_;
    CompressorControls comp_controls_;
};
