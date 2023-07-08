#include "Analyser.h"

/*---------------------------------------------------------------------------
**
*/
Analyser::Analyser(DvaManagerProcessor& processor_ref)
    : spectrogram_(processor_ref.fft_buffer_)
    , response_curve_(processor_ref.getFilterChain(), processor_ref.getSampleRate(), processor_ref.apvts_)
{
    addAndMakeVisible(&canvas_);
    addAndMakeVisible(&spectrogram_);
    addAndMakeVisible(&response_curve_);
}

/*---------------------------------------------------------------------------
**
*/
void
Analyser::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
    //    g.fillAll(juce::Colours::red.withAlpha(0.2f));
}

/*---------------------------------------------------------------------------
**
*/
void
Analyser::resized()
{
    auto bounds = getLocalBounds();

    canvas_.setBounds(bounds);
    spectrogram_.setBounds(bounds);
    response_curve_.setBounds(bounds);
}

/*---------------------------------------------------------------------------
** End of File
*/
