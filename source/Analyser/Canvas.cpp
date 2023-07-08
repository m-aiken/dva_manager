#include "Canvas.h"
#include "../Utilities/GlobalConstants.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
Canvas::Canvas()
{
}

/*---------------------------------------------------------------------------
**
*/
void
Canvas::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    juce::Font font(12.f, juce::Font::bold);
    g.setFont(font);

    auto line_colour = TailwindColours::Gray600;

    // Decibel markers.
    int max_db = static_cast< int >(Global::MAX_DB);
    int min_db = static_cast< int >(Global::NEG_INF);

    for (int i = min_db; i <= max_db; i += 6) {
        auto y = juce::jmap< float >(i, min_db, max_db, bounds.getBottom(), bounds.getY());

        // Horizontal line across analyser background.
        auto alpha = (i == min_db || i == max_db) ? 0.3f : 0.2f;

        g.setColour(line_colour.withAlpha(alpha));
        g.drawLine(bounds.getX(), roundToInt(y), bounds.getRight(), roundToInt(y), 1.f);
    }

    // Frequency markers.
    for (auto i = Global::MIN_HZ; i <= Global::MAX_HZ; ++i) {
        if (frequencyToDraw(i)) {
            auto normalized_x = juce::mapFromLog10< float >(i, Global::MIN_HZ, Global::MAX_HZ);
            auto freq_x       = bounds.getX() + bounds.getWidth() * normalized_x;
            auto alpha        = shouldBeBoldLine(i) ? 0.3f : 0.1f;

            // Vertical line across analyser background.
            g.setColour(line_colour.withAlpha(alpha));
            g.drawVerticalLine(freq_x, bounds.getY(), bounds.getBottom());
        }
    }

    // Gradient background.
    g.setGradientFill(background_gradient_);
    g.fillAll();
}

/*---------------------------------------------------------------------------
**
*/
void
Canvas::resized()
{
    auto bounds = getLocalBounds();

    background_gradient_ = juce::ColourGradient(TailwindColours::Emerald300.withAlpha(0.02f),
                                                0,
                                                bounds.getBottom(),
                                                TailwindColours::Gray900.withAlpha(0.2f),
                                                0,
                                                bounds.getCentreY(),
                                                false);

    background_gradient_.addColour(0.3, TailwindColours::Emerald300.withAlpha(0.03f));
    background_gradient_.addColour(0.6, TailwindColours::Emerald300.withAlpha(0.05f));
    background_gradient_.addColour(0.9, TailwindColours::Emerald300.withAlpha(0.08f));
}

/*---------------------------------------------------------------------------
**
*/
bool
Canvas::frequencyToDraw(int freq)
{
    bool draw = false;

    draw |= (freq <= 50) && (freq % 10 == 0);
    draw |= (freq <= 200) && (freq % 20 == 0);
    draw |= (freq <= 1000) && (freq % 100 == 0);
    draw |= (freq <= 2000) && (freq % 200 == 0);
    draw |= (freq <= 5000) && (freq % 500 == 0);
    draw |= (freq % 1000 == 0);

    return draw;
}

/*---------------------------------------------------------------------------
**
*/
bool
Canvas::shouldBeBoldLine(int freq)
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
