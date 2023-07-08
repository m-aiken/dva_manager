#include "CompressorControls.h"
#include "CompressorProperties.h"

/*---------------------------------------------------------------------------
**
*/
CompressorControls::CompressorControls(juce::AudioProcessorValueTreeState& apvts)
    : input_gain_ctrl_(apvts, RotaryTypes::GAIN, Comp::INPUT_GAIN)
    , threshold_ctrl_(apvts, RotaryTypes::GAIN, Comp::THRESHOLD)
    , ratio_ctrl_(apvts, RotaryTypes::RATIO, Comp::RATIO)
    , attack_ctrl_(apvts, RotaryTypes::TIME, Comp::ATTACK)
    , release_ctrl_(apvts, RotaryTypes::TIME, Comp::RELEASE)
    , makeup_gain_ctrl_(apvts, RotaryTypes::GAIN, Comp::MAKEUP_GAIN)
    , input_gain_label_("Input")
    , threshold_label_("Threshold")
    , ratio_label_("Ratio")
    , attack_label_("Attack")
    , release_label_("Release")
    , makeup_gain_label_("Makeup")
{
    addAndMakeVisible(&input_gain_ctrl_);
    addAndMakeVisible(&threshold_ctrl_);
    addAndMakeVisible(&ratio_ctrl_);
    addAndMakeVisible(&attack_ctrl_);
    addAndMakeVisible(&release_ctrl_);
    addAndMakeVisible(&makeup_gain_ctrl_);

    addAndMakeVisible(&input_gain_label_);
    addAndMakeVisible(&threshold_label_);
    addAndMakeVisible(&ratio_label_);
    addAndMakeVisible(&attack_label_);
    addAndMakeVisible(&release_label_);
    addAndMakeVisible(&makeup_gain_label_);
}

/*---------------------------------------------------------------------------
**
*/
void
CompressorControls::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
    //    g.fillAll(juce::Colours::yellow);
}

/*---------------------------------------------------------------------------
**
*/
void
CompressorControls::resized()
{
    using Tr = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    juce::Grid grid;

    grid.templateColumns = { Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)) };
    grid.templateRows    = { Tr(Fr(1)), Tr(Fr(5)) };

    grid.items.add(juce::GridItem(input_gain_label_));
    grid.items.add(juce::GridItem(threshold_label_));
    grid.items.add(juce::GridItem(ratio_label_));
    grid.items.add(juce::GridItem(attack_label_));
    grid.items.add(juce::GridItem(release_label_));
    grid.items.add(juce::GridItem(makeup_gain_label_));

    grid.items.add(juce::GridItem(input_gain_ctrl_));
    grid.items.add(juce::GridItem(threshold_ctrl_));
    grid.items.add(juce::GridItem(ratio_ctrl_));
    grid.items.add(juce::GridItem(attack_ctrl_));
    grid.items.add(juce::GridItem(release_ctrl_));
    grid.items.add(juce::GridItem(makeup_gain_ctrl_));

    grid.setGap(juce::Grid::Px { 12 });

    grid.performLayout(getLocalBounds());
}

/*---------------------------------------------------------------------------
** End of File
*/
