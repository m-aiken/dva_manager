#include "MainView.h"
#include "../Utilities/DvaInfo.h"

/*---------------------------------------------------------------------------
**
*/
MainView::MainView(DvaManagerProcessor& p)
    : processor_ref_(p)
    , file_info_(p)
    , analyser_(p)
    , controls_container_(p.apvts_)
    , meter_group_(p)
    , waveform_(p)
    , transport_bar_(p)
{
    addAndMakeVisible(&file_info_);
    addAndMakeVisible(&analyser_);
    addAndMakeVisible(&analyser_freq_markers_);
    addAndMakeVisible(&analyser_db_markers_);
    addAndMakeVisible(&controls_container_);
    addAndMakeVisible(&meter_group_);
    addAndMakeVisible(&waveform_);
    addAndMakeVisible(&transport_bar_);

    processor_ref_.state_.addListener(this);

    // Set the first DVA as selected.
    setSelectedDva("1");
}

/*---------------------------------------------------------------------------
**
*/
MainView::~MainView()
{
    processor_ref_.state_.removeListener(this);
}

/*---------------------------------------------------------------------------
**
*/
void
MainView::paint(juce::Graphics& g)
{
    auto         control_bounds = controls_container_.getBounds();
    float        line_thickness = 2.f;
    juce::Colour border_colour  = TailwindColours::Amber50;

    float alpha = 0.12f;
    float delta = 8.f;

    for (auto i = 0; i < 6; ++i) {
        g.setColour(border_colour.withAlpha(alpha));
        g.drawRoundedRectangle(control_bounds.expanded(delta).toFloat(), delta, line_thickness);

        alpha -= 0.02f;
        delta += 2.f;
    }
}

/*---------------------------------------------------------------------------
**
*/
void
MainView::resized()
{
    auto padding              = 20;
    auto bounds               = getLocalBounds();
    auto bounds_width         = bounds.getWidth();
    auto bounds_height        = bounds.getHeight();
    auto analyser_width       = bounds_width * 0.7;
    auto file_info_width      = analyser_width * 0.7;
    auto file_info_height     = bounds_height * 0.2 - (padding * 3);
    auto controls_height      = bounds_height * 0.18 + (padding * 2);
    auto transport_bar_height = padding * 1.7;
    auto waveform_height      = padding * 2;
    auto db_marker_width      = 48;

    file_info_.setBounds(bounds.getCentreX() - (file_info_width * 0.5), padding, file_info_width, file_info_height);

    analyser_.setBounds(bounds.getCentreX() - (analyser_width * 0.5),
                        file_info_.getBottom() + (padding * 4),
                        analyser_width,
                        Global::ANALYSER_HEIGHT);

    analyser_freq_markers_.setBounds(analyser_.getX(), analyser_.getY() - padding, analyser_width, padding);

    analyser_db_markers_.setBounds(analyser_.getX() - db_marker_width,
                                   analyser_.getY() - (Global::DB_Y_OVERFLOW_PX * 0.5),
                                   db_marker_width,
                                   Global::ANALYSER_HEIGHT + Global::DB_Y_OVERFLOW_PX);

    waveform_.setBounds(analyser_.getX(), analyser_.getBottom(), analyser_width, waveform_height);

    controls_container_.setBounds(analyser_.getX(), waveform_.getBottom() + padding, analyser_width, controls_height);

    meter_group_.setBounds(analyser_.getRight() + (padding * 0.5),
                           analyser_.getY() - (Global::DB_Y_OVERFLOW_PX * 1.5),
                           bounds.getRight() - analyser_.getRight() - (padding * 4),
                           controls_container_.getBottom() - analyser_.getY() + (Global::DB_Y_OVERFLOW_PX * 1.5));

    transport_bar_.setBounds(0, bounds.getBottom() - transport_bar_height, bounds_width, transport_bar_height);
}

/*---------------------------------------------------------------------------
**
*/
void
MainView::valueTreePropertyChanged(juce::ValueTree& value_tree, const juce::Identifier& property)
{
    if (!value_tree.isValid()) {
        return;
    }

    auto selected_dva = processor_ref_.state_.getProperty("selected_dva").toString();

    setSelectedDva(selected_dva);

    if (property == juce::Identifier("selected_dva")) {
        processor_ref_.setTransportSource();
        waveform_.updateWaveform();
        transport_bar_.updateTransportState(TransportBar::STOPPED);
        transport_bar_.repaint();
    }
}

/*---------------------------------------------------------------------------
**
*/
bool
MainView::isInterestedInFileDrag(const juce::StringArray& files)
{
    if (files.isEmpty()) {
        return false;
    }

    // We only care about the first file in this context (single file drag).
    return files[0].toLowerCase().contains(".wav");
}

/*---------------------------------------------------------------------------
**
*/
void
MainView::filesDropped(const juce::StringArray& files, int x, int y)
{
    juce::ignoreUnused(x, y);

    if (files.isEmpty() || !isInterestedInFileDrag(files)) {
        return;
    }

    // We only care about the first file in this context (single file drag).
    auto       path = files[0];
    juce::File file(path);

    updateFileSelection(file);
}

/*---------------------------------------------------------------------------
**
*/
void
MainView::setSelectedDva(const juce::String& selected_dva)
{
    juce::ValueTree dva = processor_ref_.state_.getChildWithName("dva_" + selected_dva);

    if (dva.isValid()) {
        dva_data_ = dva;
        file_info_.setData(dva);
        waveform_.setData(dva);
    }
}

/*---------------------------------------------------------------------------
**
*/
void
MainView::updateFileSelection(juce::File& file)
{
    if (file == juce::File {}) {
        return;
    }

    if (dva_data_.hasProperty(DvaInfo::getXmlProperty(DvaInfo::PATH))) {
        dva_data_.setProperty(DvaInfo::getXmlProperty(DvaInfo::PATH), file.getFullPathName(), nullptr);
    }

    processor_ref_.setTransportSource();
}

/*---------------------------------------------------------------------------
** End of File
*/
