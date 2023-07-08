#include "FileButton.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
FileButton::FileButton()
    : juce::DrawableButton("file button", juce::DrawableButton::ImageFitted)
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);

    icon_ = std::make_unique< juce::DrawableImage >(
        juce::ImageFileFormat::loadFrom(BinaryData::file_png, BinaryData::file_pngSize));
}

/*---------------------------------------------------------------------------
**
*/
void
FileButton::paint(juce::Graphics& g)
{
    auto bounds        = getLocalBounds();
    auto bounds_width  = bounds.getWidth();
    auto bounds_height = bounds.getHeight();
    auto c_y           = bounds.getCentreY();

    // Background.
    g.setColour(TailwindColours::Amber50);
    g.fillRoundedRectangle(bounds.toFloat(), 4.f);

    // Folder image.
    auto image_height = bounds_height * 0.7;

    juce::Rectangle< float > image_bounds(image_height * 0.5, c_y - (image_height * 0.5), image_height, image_height);

    g.drawImage(icon_->getImage(), image_bounds);

    // Text.
    auto text_box_width = bounds_width - image_bounds.getRight() - 10;

    g.setColour(TailwindColours::Gray900);
    g.drawFittedText("File...",
                     bounds_width - text_box_width,
                     0,
                     text_box_width,
                     bounds_height,
                     juce::Justification::centredLeft,
                     1);
}

/*---------------------------------------------------------------------------
** End of File
*/
