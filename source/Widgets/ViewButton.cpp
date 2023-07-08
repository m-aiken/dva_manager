#include "ViewButton.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
ViewButton::ViewButton()
    : juce::DrawableButton("view select button", juce::DrawableButton::ImageFitted)
    , view_(Global::MAIN_VIEW)
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);

    icon_ = std::make_unique< juce::DrawableImage >(
        juce::ImageFileFormat::loadFrom(BinaryData::folder_png, BinaryData::folder_pngSize));
}

/*---------------------------------------------------------------------------
**
*/
void
ViewButton::paint(juce::Graphics& g)
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
    auto text           = (view_ == Global::MAIN_VIEW) ? "Select folder..." : "Back to DVA view...";

    g.setColour(TailwindColours::Gray900);
    g.drawFittedText(text,
                     bounds_width - text_box_width,
                     0,
                     text_box_width,
                     bounds_height,
                     juce::Justification::centredLeft,
                     1);
}

/*---------------------------------------------------------------------------
**
*/
void
ViewButton::setIcon()
{
    const char* raw_data  = (view_ == Global::MAIN_VIEW) ? BinaryData::folder_png : BinaryData::file_png;
    size_t      file_size = (view_ == Global::MAIN_VIEW) ? BinaryData::folder_pngSize : BinaryData::file_pngSize;

    icon_ = std::make_unique< juce::DrawableImage >(juce::ImageFileFormat::loadFrom(raw_data, file_size));
}

/*---------------------------------------------------------------------------
**
*/
Global::View
ViewButton::getCurrentView()
{
    return view_;
}

/*---------------------------------------------------------------------------
**
*/
void
ViewButton::setView(Global::View view)
{
    view_ = view;

    setIcon();
    repaint();
}

/*---------------------------------------------------------------------------
** End of File
*/
