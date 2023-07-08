#pragma once

#include <JuceHeader.h>

//==============================================================================
class FolderButton : public juce::DrawableButton
{
public:
    enum TEXT_POSITION { TEXT_OVER, TEXT_SIDE };

    FolderButton(TEXT_POSITION text_position);

    void paint(juce::Graphics& g) override;

private:
    std::unique_ptr< juce::DrawableImage > folder_icon_;

    TEXT_POSITION text_position_;
};
