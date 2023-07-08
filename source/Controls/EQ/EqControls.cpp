#include "EqControls.h"
#include "EqProperties.h"

/*---------------------------------------------------------------------------
**
*/
EqControls::EqControls(juce::AudioProcessorValueTreeState& apvts)
    : low_cut_("LOW CUT", apvts, Eq::LOW_CUT_ENABLED, Eq::LOW_CUT_FREQ, Eq::LOW_CUT_SLOPE, Global::LC_SLOPE_GROUP)
    , lf_band_("LF", apvts, Eq::LF_ENABLED, Eq::LF_FREQ, Eq::LF_GAIN, Eq::LF_QUALITY)
    , lmf_band_("LMF", apvts, Eq::LMF_ENABLED, Eq::LMF_FREQ, Eq::LMF_GAIN, Eq::LMF_QUALITY)
    , hmf_band_("HMF", apvts, Eq::HMF_ENABLED, Eq::HMF_FREQ, Eq::HMF_GAIN, Eq::HMF_QUALITY)
    , hf_band_("HF", apvts, Eq::HF_ENABLED, Eq::HF_FREQ, Eq::HF_GAIN, Eq::HF_QUALITY)
    , high_cut_("HIGH CUT", apvts, Eq::HIGH_CUT_ENABLED, Eq::HIGH_CUT_FREQ, Eq::HIGH_CUT_SLOPE, Global::HC_SLOPE_GROUP)
{
    addAndMakeVisible(&low_cut_);
    addAndMakeVisible(&lf_band_);
    addAndMakeVisible(&lmf_band_);
    addAndMakeVisible(&hmf_band_);
    addAndMakeVisible(&hf_band_);
    addAndMakeVisible(&high_cut_);
}

/*---------------------------------------------------------------------------
**
*/
void
EqControls::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
    //    g.fillAll(juce::Colours::red);
}

/*---------------------------------------------------------------------------
**
*/
void
EqControls::resized()
{
    using Tr = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    juce::Grid grid;

    grid.templateColumns = { Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)) };
    grid.autoRows        = Tr(Fr(1));

    grid.items.add(juce::GridItem(low_cut_));
    grid.items.add(juce::GridItem(lf_band_));
    grid.items.add(juce::GridItem(lmf_band_));
    grid.items.add(juce::GridItem(hmf_band_));
    grid.items.add(juce::GridItem(hf_band_));
    grid.items.add(juce::GridItem(high_cut_));

    grid.setGap(juce::Grid::Px { 12 });

    grid.performLayout(getLocalBounds());
}

/*---------------------------------------------------------------------------
** End of File
*/
