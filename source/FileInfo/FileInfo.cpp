#include "FileInfo.h"
#include "../Utilities/DvaInfo.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
FileInfo::FileInfo(DvaManagerProcessor& processor_ref)
    : dva_num_label_("dva number label", "DVA 0")
    , file_name_label_(CustomGridCell::HEADER, "file name label", "File Name")
    , file_name_value_(CustomGridCell::VALUE, "file name value label", "No file selected...")
    , file_path_value_("file path value label", "")
    , file_size_label_(CustomGridCell::HEADER, "file size label", "Size (MB)")
    , file_size_value_(CustomGridCell::VALUE, "file size value label", "")
    , duration_label_(CustomGridCell::HEADER, "duration label", "Duration (Seconds)")
    , duration_value_(CustomGridCell::VALUE, "duration value label", "")
    , num_channels_label_(CustomGridCell::HEADER, "num channels label", "No. Channels")
    , num_channels_value_(CustomGridCell::VALUE, "num channels value label", "")
    , sample_rate_label_(CustomGridCell::HEADER, "sample rate label", "Sample Rate (Hz)")
    , sample_rate_value_(CustomGridCell::VALUE, "sample rate value label", "")
    , processor_ref_(processor_ref)
{
    addAndMakeVisible(&open_button_);
    addAndMakeVisible(&dva_num_label_);
    addAndMakeVisible(&file_name_label_);
    addAndMakeVisible(&file_name_value_);
    addAndMakeVisible(&file_path_value_);
    addAndMakeVisible(&file_size_label_);
    addAndMakeVisible(&file_size_value_);
    addAndMakeVisible(&duration_label_);
    addAndMakeVisible(&duration_value_);
    addAndMakeVisible(&num_channels_label_);
    addAndMakeVisible(&num_channels_value_);
    addAndMakeVisible(&sample_rate_label_);
    addAndMakeVisible(&sample_rate_value_);

    open_button_.onClick = [this]() { openButtonClicked(); };

    format_manager_.registerBasicFormats();
}

/*---------------------------------------------------------------------------
**
*/
void
FileInfo::paint(juce::Graphics& g)
{
    //    g.fillAll(juce::Colours::yellow);
}

/*---------------------------------------------------------------------------
**
*/
void
FileInfo::paintOverChildren(juce::Graphics& g)
{
    g.setColour(TailwindColours::Amber50.withAlpha(0.5f));

    juce::Font font(14.f, juce::Font::bold);
    g.setFont(font);
    g.drawFittedText(dva_num_label_.getText(),
                     file_name_value_.getX() + 10,
                     file_name_value_.getY(),
                     40,
                     file_name_value_.getHeight(),
                     juce::Justification::centredLeft,
                     1);
}

/*---------------------------------------------------------------------------
**
*/
void
FileInfo::resized()
{
    using Tr          = juce::Grid::TrackInfo;
    using Fr          = juce::Grid::Fr;
    auto bounds       = getLocalBounds();
    auto bounds_width = bounds.getWidth();
    auto row_height   = bounds.getHeight() * 0.25;
    auto button_width = 80;
    auto padding      = 4;

    // File selection button.
    open_button_.setBounds(bounds.getX(), 0, button_width, row_height);

    // File name.
    file_name_value_.setBounds(open_button_.getRight() + padding, 0, bounds_width - button_width - padding, row_height);

    // File path.
    file_path_value_.setJustificationType(juce::Justification::centred);
    file_path_value_.setBounds(file_name_value_.getX(), row_height, file_name_value_.getWidth(), row_height);

    juce::Grid file_info_grid;

    file_info_grid.templateColumns = { Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)) };
    file_info_grid.autoRows        = Tr(Fr(1));

    // Headers.
    file_info_grid.items.add(juce::GridItem(file_size_label_));
    file_info_grid.items.add(juce::GridItem(duration_label_));
    file_info_grid.items.add(juce::GridItem(num_channels_label_));
    file_info_grid.items.add(juce::GridItem(sample_rate_label_));

    // Values.
    file_info_grid.items.add(juce::GridItem(file_size_value_));
    file_info_grid.items.add(juce::GridItem(duration_value_));
    file_info_grid.items.add(juce::GridItem(num_channels_value_));
    file_info_grid.items.add(juce::GridItem(sample_rate_value_));

    file_info_grid.setGap(juce::Grid::Px { 2 });

    juce::Rectangle< int > file_grid_bounds(0, row_height * 2, bounds_width, row_height * 2);

    file_info_grid.performLayout(file_grid_bounds);
}

/*---------------------------------------------------------------------------
**
*/
void
FileInfo::setData(juce::ValueTree& dva_data)
{
    if (!dva_data.isValid()) {
        return;
    }

    dva_data_ = dva_data;

    updateGridCells();
    repaint();
}

/*---------------------------------------------------------------------------
**
*/
void
FileInfo::updateGridCells()
{
    if (!dva_data_.isValid()) {
        return;
    }

    namespace DI = DvaInfo;

    if (dva_data_.hasProperty(DI::getXmlProperty(DI::DVA_NUM))) {
        setDvaNumber(dva_data_.getProperty(DI::getXmlProperty(DI::DVA_NUM)));
    }

    if (dva_data_.hasProperty(DI::getXmlProperty(DI::FILENAME))) {
        setGridCell(file_name_value_, dva_data_.getProperty(DI::getXmlProperty(DI::FILENAME)));
    }

    if (dva_data_.hasProperty(DI::getXmlProperty(DI::PATH))) {
        file_path_value_.setText(dva_data_.getProperty(DI::getXmlProperty(DI::PATH)),
                                 juce::NotificationType::dontSendNotification);
    }

    if (dva_data_.hasProperty(DI::getXmlProperty(DI::SIZE))) {
        setGridCell(file_size_value_, dva_data_.getProperty(DI::getXmlProperty(DI::SIZE)));
    }

    if (dva_data_.hasProperty(DI::getXmlProperty(DI::DURATION_SECONDS))) {
        setGridCell(duration_value_, dva_data_.getProperty(DI::getXmlProperty(DI::DURATION_SECONDS)));
    }

    if (dva_data_.hasProperty(DI::getXmlProperty(DI::NUM_CHANNELS))) {
        setGridCell(num_channels_value_, dva_data_.getProperty(DI::getXmlProperty(DI::NUM_CHANNELS)));
    }

    if (dva_data_.hasProperty(DI::getXmlProperty(DI::SAMPLE_RATE))) {
        setGridCell(sample_rate_value_, dva_data_.getProperty(DI::getXmlProperty(DI::SAMPLE_RATE)));
    }
}

/*---------------------------------------------------------------------------
**
*/
void
FileInfo::setGridCell(CustomGridCell& cell, const juce::String& new_value)
{
    cell.setText(new_value, juce::NotificationType::dontSendNotification);
}

/*---------------------------------------------------------------------------
**
*/
void
FileInfo::setDvaNumber(const juce::String& dva_num)
{
    dva_num_label_.setText("DVA " + dva_num, juce::NotificationType::dontSendNotification);
}

/*---------------------------------------------------------------------------
**
*/
void
FileInfo::openButtonClicked()
{
    if (!dva_data_.isValid()) {
        return;
    }

    file_chooser_      = std::make_unique< juce::FileChooser >("Select a (WAV) DVA file...", juce::File {}, "*.wav");
    auto chooser_flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    file_chooser_->launchAsync(chooser_flags, [this](const juce::FileChooser& fc) {
        auto file = fc.getResult();

        updateFileSelection(file);
    });
}

/*---------------------------------------------------------------------------
**
*/
void
FileInfo::updateFileSelection(juce::File& file)
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
