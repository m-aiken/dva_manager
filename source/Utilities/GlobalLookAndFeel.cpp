#include "GlobalLookAndFeel.h"
#include "TailwindColours.h"
#include "../Controls/Compressor/CompressorProperties.h"
#include "../Controls/EQ/EqProperties.h"

/*---------------------------------------------------------------------------
**
*/
GlobalLookAndFeel::GlobalLookAndFeel()
{
    //    setDefaultSansSerifTypefaceName("Avenir Next");

    setColour(juce::Label::textColourId, TailwindColours::Amber50);
    setColour(juce::TextButton::buttonColourId, TailwindColours::Gray800);
    setColour(juce::TextButton::textColourOffId, TailwindColours::Amber50);
    setColour(juce::ListBox::textColourId, TailwindColours::Amber50);
    setColour(juce::ListBox::backgroundColourId, TailwindColours::Gray800);
}

/*---------------------------------------------------------------------------
**
*/
void
GlobalLookAndFeel::drawRotarySlider(juce::Graphics& g,
                                    int             x,
                                    int             y,
                                    int             width,
                                    int             height,
                                    float           slider_pos_proportional,
                                    float           rotary_start_angle,
                                    float           rotary_end_angle,
                                    juce::Slider&   slider)
{
    auto bounds = juce::Rectangle< float >(x, y, width, height);

    juce::Colour fill_colour       = TailwindColours::Gray800;
    juce::Colour outline_colour    = TailwindColours::Red200;
    auto         outline_thickness = 3.f;
    auto         rotary_id         = slider.getTitle().getIntValue();

    switch (rotary_id) {
    case Eq::LOW_CUT_FREQ:
    case Eq::LOW_CUT_SLOPE:
    case Eq::HIGH_CUT_FREQ:
    case Eq::HIGH_CUT_SLOPE:
        outline_colour = TailwindColours::Yellow300;
        break;

    case Eq::LF_FREQ:
    case Eq::LF_GAIN:
    case Eq::LF_QUALITY:
        outline_colour = TailwindColours::Red700;
        break;

    case Eq::LMF_FREQ:
    case Eq::LMF_GAIN:
    case Eq::LMF_QUALITY:
        outline_colour = TailwindColours::Blue300;
        break;

    case Eq::HMF_FREQ:
    case Eq::HMF_GAIN:
    case Eq::HMF_QUALITY:
        outline_colour = TailwindColours::Emerald400;
        break;

    case Eq::HF_FREQ:
    case Eq::HF_GAIN:
    case Eq::HF_QUALITY:
        outline_colour = TailwindColours::Red300;
        break;
    }

    if (!slider.isEnabled()) {
        outline_colour = TailwindColours::Gray600;
    }

    //    juce::ColourGradient gradient(TailwindColours::Gray500,
    //                                  bounds.getCentreX(),
    //                                  bounds.getCentreY(),
    //                                  TailwindColours::Gray900,
    //                                  x,
    //                                  y,
    //                                  true);
    //
    //    gradient.addColour(0.25f, TailwindColours::Gray600);
    //    gradient.addColour(0.5f, TailwindColours::Gray700);
    //    gradient.addColour(0.75f, TailwindColours::Gray800);
    //
    //    g.setGradientFill(gradient);

    // Rotary Fill.
    g.setColour(fill_colour);
    g.fillEllipse(bounds.reduced(outline_thickness));

    // Outline path.
    auto radius     = juce::jmin(width, height) * 0.5f;
    auto arc_radius = radius - (outline_thickness * 0.5f);

    g.setColour(outline_colour);
    juce::Path value_arc;

    value_arc.addCentredArc(bounds.getCentreX(),
                            bounds.getCentreY(),
                            arc_radius,
                            arc_radius,
                            0.f,
                            (rotary_id == Eq::HIGH_CUT_FREQ) ? slider_pos_proportional : rotary_start_angle,
                            (rotary_id == Eq::HIGH_CUT_FREQ) ? rotary_end_angle : slider_pos_proportional,
                            true);

    g.strokePath(value_arc, juce::PathStrokeType(outline_thickness));
}

/*---------------------------------------------------------------------------
**
*/
void
GlobalLookAndFeel::drawToggleButton(juce::Graphics&     g,
                                    juce::ToggleButton& button,
                                    bool                should_draw_button_as_highlighted,
                                    bool                should_draw_button_as_down)
{
    juce::ignoreUnused(should_draw_button_as_highlighted, should_draw_button_as_down);

    juce::Colour fill_colour = button.getToggleState() ? TailwindColours::Red400 : TailwindColours::Gray800;
    juce::Colour text_colour = TailwindColours::Amber50;

    g.setColour(fill_colour);
    g.fillRoundedRectangle(g.getClipBounds().toFloat(), Global::BUTTON_CORNER_RADIUS);

    g.setColour(text_colour);

    g.drawFittedText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, 1);
}

/*---------------------------------------------------------------------------
** End of File
*/
