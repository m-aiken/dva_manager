#pragma once

#include <JuceHeader.h>

//==============================================================================
class CustomGridCell : public juce::Label
{
public:
    enum CELL_TYPE { HEADER, VALUE };

    CustomGridCell(CELL_TYPE           cell_type,
                   const juce::String& component_name = juce::String(),
                   const juce::String& label_text     = juce::String());

    void paint(juce::Graphics& g) override;

private:
    CELL_TYPE cell_type_;
};
