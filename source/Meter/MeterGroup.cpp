#include "MeterGroup.h"
#include "../Utilities/GlobalConstants.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
MeterGroup::MeterGroup(DvaManagerProcessor& processor_ref)
    : peak_select_btn_("Peak")
    , rms_select_btn_("RMS")
    , lufs_select_btn_("LUFS")
    , peak_meter_([&]() { return processor_ref.getPeakValue(); })
    , rms_meter_([&]() { return processor_ref.getRmsValue(); })
    , lufs_meter_([&]() { return processor_ref.getLufsValue(); })
    , peak_value_([&]() { return processor_ref.getPeakValue(); })
    , rms_value_([&]() { return processor_ref.getRmsValue(); })
    , lufs_value_([&]() { return processor_ref.getLufsValue(); })
{
    addAndMakeVisible(&peak_select_btn_);
    addAndMakeVisible(&rms_select_btn_);
    addAndMakeVisible(&lufs_select_btn_);
    addAndMakeVisible(&peak_meter_);
    addAndMakeVisible(&rms_meter_);
    addAndMakeVisible(&lufs_meter_);
    addAndMakeVisible(&db_markers_);
    addAndMakeVisible(&peak_value_);
    addAndMakeVisible(&rms_value_);
    addAndMakeVisible(&lufs_value_);

    peak_select_btn_.setRadioGroupId(Global::METER_SELECT_GROUP);
    rms_select_btn_.setRadioGroupId(Global::METER_SELECT_GROUP);
    lufs_select_btn_.setRadioGroupId(Global::METER_SELECT_GROUP);

    peak_select_btn_.onClick = [&]() { setView(&peak_select_btn_, PEAK_INDEX); };
    rms_select_btn_.onClick  = [&]() { setView(&rms_select_btn_, RMS_INDEX); };
    lufs_select_btn_.onClick = [&]() { setView(&lufs_select_btn_, LUFS_INDEX); };

    peak_select_btn_.setToggleState(true, juce::NotificationType::sendNotification);
}

/*---------------------------------------------------------------------------
**
*/
void
MeterGroup::paint(juce::Graphics& g)
{
    g.setColour(TailwindColours::Gray900.withAlpha(0.2f));
    //    g.setColour(TailwindColours::Gray500);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.f);
}

/*---------------------------------------------------------------------------
**
*/
void
MeterGroup::resized()
{
    using Tr = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    auto bounds           = getLocalBounds();
    auto bounds_width     = bounds.getWidth();
    auto value_box_margin = 12;

    // Current value.
    juce::Rectangle< int > value_bounds(value_box_margin,
                                        value_box_margin,
                                        bounds_width - (value_box_margin * 2),
                                        Global::DB_Y_OVERFLOW_PX * 0.5);

    peak_value_.setBounds(value_bounds);
    rms_value_.setBounds(value_bounds);
    lufs_value_.setBounds(value_bounds);

    // Meter.
    auto                   meter_width = 10;
    juce::Rectangle< int > meter_bounds(bounds_width * 0.25,
                                        Global::DB_Y_OVERFLOW_PX * 1.5,
                                        meter_width,
                                        Global::ANALYSER_HEIGHT);

    peak_meter_.setBounds(meter_bounds);
    rms_meter_.setBounds(meter_bounds);
    lufs_meter_.setBounds(meter_bounds);

    // dB markers.
    auto                   db_scale_width = bounds_width * 0.5;
    juce::Rectangle< int > db_scale_bounds(bounds_width * 0.5,
                                           Global::DB_Y_OVERFLOW_PX,
                                           db_scale_width,
                                           Global::ANALYSER_HEIGHT + Global::DB_Y_OVERFLOW_PX);

    db_markers_.setBounds(db_scale_bounds);

    // Buttons.
    juce::Grid button_grid;

    button_grid.autoColumns  = Tr(Fr(1));
    button_grid.templateRows = { Tr(Fr(1)), Tr(Fr(1)), Tr(Fr(1)) };

    button_grid.items.add(juce::GridItem(peak_select_btn_));
    button_grid.items.add(juce::GridItem(rms_select_btn_));
    button_grid.items.add(juce::GridItem(lufs_select_btn_));

    button_grid.setGap(juce::Grid::Px { 8 });

    auto                   button_width  = bounds.getWidth() * 0.8;
    auto                   button_height = 32;
    juce::Rectangle< int > button_bounds(bounds.getCentreX() - (button_width * 0.5),
                                         meter_bounds.getBottom() + button_height,
                                         button_width,
                                         button_height * 3);

    button_grid.performLayout(button_bounds);
}

/*---------------------------------------------------------------------------
**
*/
void
MeterGroup::setView(juce::ToggleButton* button, uint8_t new_index)
{
    if ((button == nullptr) || !button->getToggleState()) {
        return;
    }

    peak_meter_.setVisible(new_index == PEAK_INDEX);
    peak_value_.setVisible(new_index == PEAK_INDEX);

    rms_meter_.setVisible(new_index == RMS_INDEX);
    rms_value_.setVisible(new_index == RMS_INDEX);

    lufs_meter_.setVisible(new_index == LUFS_INDEX);
    lufs_value_.setVisible(new_index == LUFS_INDEX);
}

/*---------------------------------------------------------------------------
** End of File
*/
