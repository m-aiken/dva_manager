#pragma once

#include <JuceHeader.h>

//==============================================================================
class FileButton : public juce::DrawableButton
{
public:
    FileButton();

    void paint(juce::Graphics& g) override;

private:
    std::unique_ptr< juce::DrawableImage > icon_;
};
