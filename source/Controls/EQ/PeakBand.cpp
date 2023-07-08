#include "PeakBand.h"
#include "../../Utilities/TailwindColours.h"
#include "../../Widgets/RotaryTypes.h"

/*---------------------------------------------------------------------------
**
*/
PeakBand::PeakBand(const juce::String&                 label_text,
                   juce::AudioProcessorValueTreeState& apvts,
                   Eq::Control                         enabled_ctrl,
                   Eq::Control                         freq_ctrl,
                   Eq::Control                         gain_ctrl,
                   Eq::Control                         q_ctrl)
    : label_(label_text)
    , enabled_btn_(apvts.getParameter(Eq::str(enabled_ctrl)))
    , freq_(apvts, RotaryTypes::FREQUENCY, freq_ctrl, enabled_ctrl)
    , gain_(apvts, RotaryTypes::GAIN, gain_ctrl, enabled_ctrl)
    , q_(apvts, RotaryTypes::Q, q_ctrl, enabled_ctrl)
{
    addAndMakeVisible(&label_);
    addAndMakeVisible(&enabled_btn_);
    addAndMakeVisible(&freq_);
    addAndMakeVisible(&gain_);
    addAndMakeVisible(&q_);
}

/*---------------------------------------------------------------------------
**
*/
void
PeakBand::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
    //    g.setColour(TailwindColours::Gray800);
    //    g.drawRect(getLocalBounds(), 1.f);
    //    g.fillAll(TailwindColours::Orange300);
}

/*---------------------------------------------------------------------------
**
*/
void
PeakBand::resized()
{
    auto bounds          = getLocalBounds();
    auto label_height    = bounds.getHeight() / 8;
    auto rotary_diameter = (bounds.getHeight() - label_height) * 0.55;
    auto q_diameter      = (bounds.getHeight() - label_height) * 0.4;
    auto padding         = 6;

    label_.setBounds(0, 0, bounds.getWidth(), label_height);

    enabled_btn_.setBounds(0, 0, label_height * 1.8, label_height);

    freq_.setBounds(bounds.getCentreX() - rotary_diameter - padding,
                    label_height + (padding * 2),
                    rotary_diameter,
                    rotary_diameter);
    gain_.setBounds(bounds.getCentreX() + padding, label_height + (padding * 2), rotary_diameter, rotary_diameter);
    q_.setBounds(bounds.getCentreX() - (q_diameter * 0.5), bounds.getBottom() - q_diameter, q_diameter, q_diameter);
}

/*---------------------------------------------------------------------------
** End of File
*/
