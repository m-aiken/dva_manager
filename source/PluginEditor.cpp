#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utilities/GlobalConstants.h"
#include "Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
DvaManagerEditor::DvaManagerEditor(DvaManagerProcessor& p)
    : AudioProcessorEditor(&p)
    , processorRef(p)
    , slot_browser_(p.state_)
    , main_view_(p)
    , folder_view_(p)
{
    setLookAndFeel(&lnf_);

    addAndMakeVisible(&slot_browser_);
    addAndMakeVisible(&main_view_);
    addAndMakeVisible(&folder_view_);
    addAndMakeVisible(&view_button_);

    // Main window size.
    setSize(1400, 900);

    folder_view_.setVisible(false);

    view_button_.onClick = [this]() { handleViewSwitch(); };
}

/*---------------------------------------------------------------------------
**
*/
DvaManagerEditor::~DvaManagerEditor()
{
    setLookAndFeel(nullptr);
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerEditor::paint(juce::Graphics& g)
{
    // Main background.
    g.fillAll(TailwindColours::Gray800);
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerEditor::resized()
{
    auto bounds             = getLocalBounds();
    auto bounds_width       = bounds.getWidth();
    auto bounds_height      = bounds.getHeight();
    auto slot_browser_width = bounds.getWidth() * 0.12;

    slot_browser_.setBounds(0, 0, slot_browser_width, bounds_height);
    main_view_.setBounds(slot_browser_width, 0, bounds_width - slot_browser_width, bounds_height);
    folder_view_.setBounds(slot_browser_width, 0, bounds_width - slot_browser_width, bounds_height);
    view_button_.setBounds(bounds_width - 180, 20, 160, 32);
}

/*---------------------------------------------------------------------------
**
*/
void
DvaManagerEditor::handleViewSwitch()
{
    Global::View view_at_click_time = view_button_.getCurrentView();

    if (view_at_click_time == Global::MAIN_VIEW) {
        main_view_.setVisible(false);
        folder_view_.setVisible(true);

        folder_view_.status_label_ = "";

        view_button_.setView(Global::FOLDER_VIEW);
    }
    else {
        main_view_.setVisible(true);
        folder_view_.setVisible(false);

        processorRef.setTransportSource();

        view_button_.setView(Global::MAIN_VIEW);
    }
}

/*---------------------------------------------------------------------------
** End of File
*/
