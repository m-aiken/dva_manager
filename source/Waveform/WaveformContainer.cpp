#include "WaveformContainer.h"
#include "../Utilities/DvaInfo.h"

/*---------------------------------------------------------------------------
**
*/
WaveformContainer::WaveformContainer(DvaManagerProcessor& processor_ref)
    : waveform_cache_(5)
    , waveform_(512, processor_ref.getFormatManager(), waveform_cache_)
    , position_marker_(waveform_, processor_ref.getTransportSource())
{
    addAndMakeVisible(&waveform_);
    addAndMakeVisible(&position_marker_);
}

/*---------------------------------------------------------------------------
**
*/
void
WaveformContainer::resized()
{
    auto bounds = getLocalBounds();

    waveform_.setBounds(bounds);
    position_marker_.setBounds(bounds);
}

/*---------------------------------------------------------------------------
**
*/
void
WaveformContainer::setData(juce::ValueTree& dva_data)
{
    dva_data_ = dva_data;

    updateWaveform();
}

/*---------------------------------------------------------------------------
**
*/
void
WaveformContainer::updateWaveform()
{
    if (!dva_data_.isValid() || !dva_data_.hasProperty(DvaInfo::getXmlProperty(DvaInfo::PATH))) {
        return;
    }

    juce::File file(dva_data_.getProperty(DvaInfo::getXmlProperty(DvaInfo::PATH)));

    waveform_.setSource((file == juce::File {}) ? nullptr : new juce::FileInputSource(file));
    waveform_.repaint();
}

/*---------------------------------------------------------------------------
** End of File
*/
