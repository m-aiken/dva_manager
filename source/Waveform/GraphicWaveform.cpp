#include "GraphicWaveform.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
GraphicWaveform::GraphicWaveform(int num_samples, juce::AudioFormatManager& format_manager, juce::AudioThumbnailCache& cache)
    : juce::AudioThumbnail(num_samples, format_manager, cache)
{
}

/*---------------------------------------------------------------------------
**
*/
void
GraphicWaveform::paint(juce::Graphics& g)
{
    if (getNumChannels() == 0) {
        return;
    }

    auto bounds       = getLocalBounds();
    auto audio_length = static_cast< float >(getTotalLength());

    g.setColour(TailwindColours::Amber50.withAlpha(0.3f));
    //        waveform_.drawChannel(g, waveform_bounds, 0.0, waveform_.getTotalLength(), 0, 1.0f);
    drawChannels(g, bounds, 0.0, audio_length, 1.0f);
}

/*---------------------------------------------------------------------------
** End of File
*/
