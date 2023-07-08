#pragma once

#include <JuceHeader.h>

#include "../Utilities/GlobalConstants.h"

//==============================================================================
class ViewButton : public juce::DrawableButton
{
public:
    ViewButton();

    void paint(juce::Graphics& g) override;

    void setIcon();

    Global::View getCurrentView();
    void         setView(Global::View view);

private:
    std::unique_ptr< juce::DrawableImage > icon_;
    Global::View                           view_;
};
