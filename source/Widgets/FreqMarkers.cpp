#include "FreqMarkers.h"
#include "../Utilities/GlobalConstants.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
FreqMarkers::FreqMarkers()
{
}

/*---------------------------------------------------------------------------
**
*/
void
FreqMarkers::paint(juce::Graphics& g)
{
    //    g.fillAll(TailwindColours::Yellow400);

    auto bounds           = getLocalBounds();
    auto text_block_width = bounds.getWidth() * 0.1;

    juce::Font font(12.f, juce::Font::bold);
    g.setFont(font);

    g.setColour(TailwindColours::Amber50);

    for (auto i = Global::MIN_HZ; i <= Global::MAX_HZ; ++i) {
        if (inCatchGroup(i)) {
            auto normalized_x = juce::mapFromLog10< float >(i, Global::MIN_HZ, Global::MAX_HZ);
            auto freq_x       = bounds.getX() + bounds.getWidth() * normalized_x;
            // Adjust the x coordinate to center the text.
            auto text_x = freq_x - (text_block_width * 0.5);

            auto freq   = (i >= 1000) ? i / 1000 : i;
            auto suffix = (i >= 1000) ? "kHz" : "Hz";

            g.drawFittedText(juce::String(freq) + suffix,
                             text_x,
                             bounds.getY(),
                             text_block_width,
                             bounds.getHeight(),
                             juce::Justification::centred,
                             1);
        }
    }
}

/*---------------------------------------------------------------------------
**
*/
bool
FreqMarkers::inCatchGroup(int freq)
{
    bool bold = false;

    bold |= (freq == 50);
    bold |= (freq == 100);
    bold |= (freq == 200);
    bold |= (freq == 500);
    bold |= (freq == 1000);
    bold |= (freq == 2000);
    bold |= (freq == 5000);
    bold |= (freq == 10000);

    return bold;
}

/*---------------------------------------------------------------------------
** End of File
*/
