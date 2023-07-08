#include "Meter.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
Meter::Meter(std::function< float() >&& value_getter_fn)
    : getValueFn(std::move(value_getter_fn))
{
    startTimerHz(60);
}

/*---------------------------------------------------------------------------
**
*/
Meter::~Meter()
{
    stopTimer();
}

/*---------------------------------------------------------------------------
**
*/
void
Meter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Background.
    g.fillAll(TailwindColours::Amber50.withAlpha(0.2f));

    // Value bar.
    auto scaled_y = juce::jmap< float >(getValueFn(), Global::NEG_INF, Global::MAX_DB, bounds.getBottom(), bounds.getY());

    juce::Rectangle< float > val_rect(bounds.getX(), scaled_y, bounds.getWidth(), bounds.getHeight() - scaled_y);

    g.setGradientFill(colour_gradient_);
    g.fillRect(val_rect);

    // Decibel markers.
    g.setColour(TailwindColours::Gray900);

    for (auto i = Global::NEG_INF; i <= Global::MAX_DB; i += 6) {
        auto y = juce::jmap< float >(i, Global::NEG_INF, Global::MAX_DB, bounds.getBottom(), bounds.getY());

        g.drawLine(0, roundToInt(y), bounds.getWidth(), y, 1.f);
    }

    // Border.
    g.drawRect(bounds, 1.f);
}

/*---------------------------------------------------------------------------
**
*/
void
Meter::resized()
{
    auto bounds = getLocalBounds();

    colour_gradient_ = juce::ColourGradient(TailwindColours::Green400,
                                            bounds.getX(),
                                            bounds.getBottom(),
                                            TailwindColours::Red400,
                                            bounds.getWidth(),
                                            bounds.getY(),
                                            false);

    colour_gradient_.addColour(0.5, TailwindColours::Yellow400);
}

/*---------------------------------------------------------------------------
**
*/
void
Meter::timerCallback()
{
    repaint();
}

/*---------------------------------------------------------------------------
** End of File
*/
