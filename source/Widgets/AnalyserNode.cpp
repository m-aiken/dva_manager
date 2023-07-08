#include "AnalyserNode.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
AnalyserNode::AnalyserNode(juce::AudioProcessorValueTreeState& apvts, Eq::Control band_control)
    : param_attachment_(apvts, Eq::str(band_control), *this)
{
}

/*---------------------------------------------------------------------------
**
*/
void
AnalyserNode::paint(juce::Graphics& g)
{
    g.setColour(getToggleState() ? TailwindColours::Emerald300 : TailwindColours::Gray800);
    g.fillEllipse(getLocalBounds().toFloat());
}

/*---------------------------------------------------------------------------
** End of File
*/
