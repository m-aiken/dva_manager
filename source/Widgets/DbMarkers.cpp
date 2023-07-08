#include "DbMarkers.h"
#include "../Utilities/GlobalConstants.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
DbMarkers::DbMarkers()
{
}

/*---------------------------------------------------------------------------
**
*/
void
DbMarkers::paint(juce::Graphics& g)
{
    //    g.fillAll(TailwindColours::Teal400);

    // The bounds given to this component (init_b) have a y overflow factored in.
    // This allows for the correct center positioning of each text item on the y-axis.
    // Compensate for this for our drawable bounds.
    auto init_b          = getLocalBounds();
    auto init_width      = init_b.getWidth();
    auto init_height     = init_b.getHeight();
    auto drawable_bounds = init_b.withSizeKeepingCentre(init_width, init_height - Global::DB_Y_OVERFLOW_PX);

    juce::Font font(12.f, juce::Font::bold);
    g.setFont(font);

    g.setColour(TailwindColours::Amber50);

    int max_db = static_cast< int >(Global::MAX_DB);
    int min_db = static_cast< int >(Global::NEG_INF);

    for (int i = min_db; i <= max_db; i += 6) {
        auto y = juce::jmap< float >(i, min_db, max_db, drawable_bounds.getBottom(), drawable_bounds.getY());

        // Adjust the y coordinate to center the text.
        y -= (font.getHeight() * 0.5);

        auto value_text = ((i > 0) ? "+" : "") + juce::String(i) + "dB";

        g.drawFittedText(value_text,
                         0,
                         roundToInt(y),
                         drawable_bounds.getWidth(),
                         font.getHeight(),
                         juce::Justification::centred,
                         1);
    }
}

/*---------------------------------------------------------------------------
** End of File
*/
