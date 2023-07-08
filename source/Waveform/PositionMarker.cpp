#include "PositionMarker.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
PositionMarker::PositionMarker(GraphicWaveform& waveform, juce::AudioTransportSource& transport_source)
    : waveform_(waveform)
    , transport_source_(transport_source)
{
    startTimerHz(60);
}

/*---------------------------------------------------------------------------
**
*/
PositionMarker::~PositionMarker()
{
    stopTimer();
}

/*---------------------------------------------------------------------------
**
*/
void
PositionMarker::paint(juce::Graphics& g)
{
    if (waveform_.getNumChannels() == 0) {
        // No file loaded.
        return;
    }

    g.setColour(TailwindColours::Red500);

    auto bounds         = getLocalBounds();
    auto audio_length   = static_cast< float >(waveform_.getTotalLength());
    auto audio_position = static_cast< float >(transport_source_.getCurrentPosition());
    auto line_position  = (audio_position / audio_length) * static_cast< float >(bounds.getWidth())
                         + static_cast< float >(bounds.getX());

    g.drawLine(line_position,
               static_cast< float >(bounds.getY()),
               line_position,
               static_cast< float >(bounds.getBottom()),
               1.f);
}

/*---------------------------------------------------------------------------
**
*/
void
PositionMarker::timerCallback()
{
    repaint();
}

/*---------------------------------------------------------------------------
** End of File
*/
