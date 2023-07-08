#include "FolderButton.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
FolderButton::FolderButton(TEXT_POSITION text_position)
    : juce::DrawableButton("folder", juce::DrawableButton::ImageFitted)
    , text_position_(text_position)
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);

    folder_icon_ = std::make_unique< juce::DrawableImage >(
        juce::ImageFileFormat::loadFrom(BinaryData::folder_png, BinaryData::folder_pngSize));
}

/*---------------------------------------------------------------------------
**
*/
void
FolderButton::paint(juce::Graphics& g)
{
    auto bounds        = getLocalBounds();
    auto bounds_width  = bounds.getWidth();
    auto bounds_height = bounds.getHeight();
    auto c_x           = bounds.getCentreX();
    auto c_y           = bounds.getCentreY();

    // Background.
    g.setColour(TailwindColours::Amber50);
    g.fillRoundedRectangle(bounds.toFloat(), (text_position_ == TEXT_OVER) ? 10.f : 8.f);

    // Folder image.
    auto image_width  = bounds_width * 0.7;
    auto image_height = bounds_height * 0.7;

    juce::Rectangle< float > image_bounds_centered(c_x - (image_width * 0.5),
                                                   c_y - (image_height * 0.5),
                                                   image_width,
                                                   image_height);

    juce::Rectangle< float > image_bounds_side(image_height * 0.5, c_y - (image_height * 0.5), image_height, image_height);

    g.drawImage(folder_icon_->getImage(), (text_position_ == TEXT_OVER) ? image_bounds_centered : image_bounds_side);

    // Text.
    if (text_position_ == TEXT_OVER) {
        juce::Font font(18.f, juce::Font::bold);
        g.setFont(font);
        g.drawFittedText("Select...", image_bounds_centered.toNearestInt(), juce::Justification::centred, 1);
    }
    else {
        g.setColour(TailwindColours::Gray900);
        g.drawFittedText("Select folder...",
                         image_bounds_side.getRight(),
                         0,
                         bounds_width - image_bounds_side.getRight(),
                         bounds_height,
                         juce::Justification::centred,
                         1);
    }
}

/*---------------------------------------------------------------------------
** End of File
*/
