#pragma once

#include <JuceHeader.h>

#include "Meter.h"
#include "MeterValueLabel.h"
#include "../PluginProcessor.h"
#include "../Widgets/DbMarkers.h"
#include "../Widgets/RadioButton.h"

//==============================================================================
class MeterGroup : public juce::Component
{
public:
    MeterGroup(DvaManagerProcessor& processor_ref);

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setView(juce::ToggleButton* button, uint8_t new_index);

private:
    enum { PEAK_INDEX = 0, RMS_INDEX, LUFS_INDEX };

    RadioButton peak_select_btn_;
    RadioButton rms_select_btn_;
    RadioButton lufs_select_btn_;

    Meter peak_meter_;
    Meter rms_meter_;
    Meter lufs_meter_;

    DbMarkers db_markers_;

    MeterValueLabel peak_value_;
    MeterValueLabel rms_value_;
    MeterValueLabel lufs_value_;
};
