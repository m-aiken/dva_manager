#include "../Utilities/TailwindColours.h"
#include "TransportBar.h"

/*---------------------------------------------------------------------------
**
*/
TransportBar::TransportBar(DvaManagerProcessor& processor_ref)
    : processor_ref_(processor_ref)
    , transport_source_(processor_ref.getTransportSource())
    , transport_state_(STOPPED)
    , play_button_("Play", TailwindColours::Green500, TailwindColours::Green400)
    , stop_button_("Stop", TailwindColours::Red500, TailwindColours::Red400)
    , loop_button_("Loop", TailwindColours::Yellow500, TailwindColours::Yellow400)
{
    play_icon_ = std::make_unique< juce::DrawableImage >(
        juce::ImageFileFormat::loadFrom(BinaryData::play_png, BinaryData::play_pngSize));
    pause_icon_ = std::make_unique< juce::DrawableImage >(
        juce::ImageFileFormat::loadFrom(BinaryData::pause_png, BinaryData::pause_pngSize));
    stop_icon_ = std::make_unique< juce::DrawableImage >(
        juce::ImageFileFormat::loadFrom(BinaryData::stop_png, BinaryData::stop_pngSize));
    loop_icon_ = std::make_unique< juce::DrawableImage >(
        juce::ImageFileFormat::loadFrom(BinaryData::loop_png, BinaryData::loop_pngSize));

    addAndMakeVisible(&play_button_);
    play_button_.setImages(play_icon_.get());
    play_button_.onClick = [this]() { playButtonClicked(); };

    addAndMakeVisible(&stop_button_);
    stop_button_.setImages(stop_icon_.get());
    stop_button_.onClick = [this]() { stopButtonClicked(); };

    addAndMakeVisible(&loop_button_);
    loop_button_.setImages(loop_icon_.get());
    loop_button_.onClick = [this]() { loopButtonClicked(); };

    transport_source_.addChangeListener(this);
}

/*---------------------------------------------------------------------------
**
*/
void
TransportBar::paint(juce::Graphics& g)
{
    // Horizontal line across top of bounds.
    g.setColour(TailwindColours::Gray800);
    g.drawLine(0, 0, getLocalBounds().getWidth(), 0, 2.f);

    // Loop state.
    juce::AudioFormatReaderSource* reader_source = processor_ref_.getReaderSource();

    if (reader_source == nullptr) {
        // reader_source will be nullptr if no file has been selected.
        return;
    }

    if (reader_source->isLooping()) {
        g.setColour(TailwindColours::Green500);
        auto loop_btn          = loop_button_.getBounds();
        auto line_thickness    = 2.f;
        auto highlight_padding = line_thickness * 2;

        g.drawRoundedRectangle(loop_btn.getX() - highlight_padding,
                               loop_btn.getY() - highlight_padding,
                               loop_btn.getWidth() + (highlight_padding * 2),
                               loop_btn.getHeight() + (highlight_padding * 2),
                               4.f,
                               line_thickness);
    }
}

/*---------------------------------------------------------------------------
**
*/
void
TransportBar::resized()
{
    using Tr        = juce::Grid::TrackInfo;
    using Fr        = juce::Grid::Fr;
    auto bounds     = getLocalBounds();
    auto grid_width = bounds.getWidth() * 0.15;
    auto padding    = 8;

    juce::Grid grid;

    grid.templateColumns = { Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)) };
    grid.autoRows        = Tr(Fr(1));

    grid.items.add(juce::GridItem(play_button_));
    grid.items.add(juce::GridItem(stop_button_));
    grid.items.add(juce::GridItem(loop_button_));

    grid.setGap(juce::Grid::Px { padding });

    juce::Rectangle< int > grid_bounds(bounds.getCentreX() - (grid_width * 0.5),
                                       padding,
                                       grid_width,
                                       bounds.getHeight() - (padding * 2));

    grid.performLayout(grid_bounds);
}

/*---------------------------------------------------------------------------
**
*/
void
TransportBar::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == nullptr) {
        return;
    }

    if (source == &transport_source_) {
        if (transport_source_.hasStreamFinished()) {
            updateTransportState(STOPPED);
        }
    }
}

/*---------------------------------------------------------------------------
**
*/
void
TransportBar::updateTransportState(TRANSPORT_STATE new_state)
{
    if (transport_state_ == new_state) {
        return;
    }

    transport_state_ = new_state;

    switch (transport_state_) {
    case STOPPED:
        transport_source_.stop();
        transport_source_.setPosition(0.0);

        if (play_icon_ != nullptr) {
            play_button_.setImages(play_icon_.get());
        }

        break;

    case PLAYING:
        transport_source_.start();

        if (pause_icon_ != nullptr) {
            play_button_.setImages(pause_icon_.get());
        }

        break;

    case PAUSED:
        transport_source_.stop();

        if (play_icon_ != nullptr) {
            play_button_.setImages(play_icon_.get());
        }

        break;
    }
}

/*---------------------------------------------------------------------------
**
*/
void
TransportBar::playButtonClicked()
{
    if ((transport_state_ == STOPPED) || (transport_state_ == PAUSED)) {
        updateTransportState(PLAYING);
    }
    else if (transport_state_ == PLAYING) {
        updateTransportState(PAUSED);
    }
}

/*---------------------------------------------------------------------------
**
*/
void
TransportBar::stopButtonClicked()
{
    updateTransportState(STOPPED);
}

/*---------------------------------------------------------------------------
**
*/
void
TransportBar::loopButtonClicked()
{
    juce::AudioFormatReaderSource* reader_source = processor_ref_.getReaderSource();

    if (reader_source == nullptr) {
        // reader_source will be nullptr if no file has been selected.
        return;
    }

    reader_source->setLooping(!reader_source->isLooping());

    repaint();
}

/*---------------------------------------------------------------------------
** End of File
*/
