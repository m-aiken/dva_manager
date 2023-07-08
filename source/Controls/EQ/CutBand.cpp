#include "CutBand.h"
#include "../../Utilities/TailwindColours.h"
#include "../../Widgets/RotaryTypes.h"

/*---------------------------------------------------------------------------
**
*/
CutBand::CutBand(const juce::String&                 label_text,
                 juce::AudioProcessorValueTreeState& apvts,
                 Eq::Control                         enabled_ctrl,
                 Eq::Control                         freq_ctrl,
                 Eq::Control                         slope_ctrl,
                 Global::RadioGroup                  radio_group)
    : label_(label_text)
    , enabled_btn_(apvts.getParameter(Eq::str(enabled_ctrl)))
    , freq_(apvts, RotaryTypes::FREQUENCY, freq_ctrl, enabled_ctrl)
    , slope_radios_(apvts.getParameter(Eq::str(slope_ctrl)), radio_group)
    , slope_ctrl_(slope_ctrl)
{
    addAndMakeVisible(&label_);
    addAndMakeVisible(&enabled_btn_);
    addAndMakeVisible(&freq_);
    addAndMakeVisible(&slope_radios_);
}

/*---------------------------------------------------------------------------
**
*/
void
CutBand::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
    //    g.fillAll(TailwindColours::Yellow400);
}

/*---------------------------------------------------------------------------
**
*/
void
CutBand::resized()
{
    auto bounds             = getLocalBounds();
    auto label_height       = bounds.getHeight() / 8;
    auto rotary_diameter    = bounds.getWidth() * 0.5;
    auto slope_button_width = bounds.getWidth() * 0.2;
    auto slope_x_padding    = slope_button_width * 0.5;
    auto padding            = 6;

    label_.setBounds(0, 0, bounds.getWidth(), label_height);

    enabled_btn_.setBounds(0, 0, label_height * 1.8, label_height);

    auto freq_x_init = bounds.getCentreX() - (rotary_diameter * 0.5);
    auto freq_x      = slope_ctrl_ == Eq::LOW_CUT_SLOPE ? freq_x_init + slope_x_padding : freq_x_init - slope_x_padding;

    auto freq_centre_y = ((bounds.getHeight() - label_height) * 0.5) + label_height;
    freq_.setBounds(freq_x, freq_centre_y - (rotary_diameter * 0.5), rotary_diameter, rotary_diameter);

    slope_radios_.setBounds(slope_ctrl_ == Eq::LOW_CUT_SLOPE ? 0 : bounds.getRight() - slope_button_width,
                            label_height + (padding * 2),
                            slope_button_width,
                            bounds.getHeight() - label_height - (padding * 2));
}

/*---------------------------------------------------------------------------
** End of File
*/
