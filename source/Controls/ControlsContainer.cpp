#include "ControlsContainer.h"
#include "../Utilities/TailwindColours.h"

/*---------------------------------------------------------------------------
**
*/
ControlsContainer::ControlsContainer(juce::AudioProcessorValueTreeState& apvts)
    : eq_enabled_btn_(apvts.getParameter(Eq::str(Eq::ENABLED)))
    , comp_enabled_btn_(apvts.getParameter(Comp::str(Comp::ENABLED)))
    , eq_select_btn_("EQ")
    , comp_select_btn_("Comp")
    , eq_controls_(apvts)
    , comp_controls_(apvts)
{
    addAndMakeVisible(&eq_enabled_btn_);
    addAndMakeVisible(&comp_enabled_btn_);
    addAndMakeVisible(&eq_select_btn_);
    addAndMakeVisible(&comp_select_btn_);
    addAndMakeVisible(&eq_controls_);
    addAndMakeVisible(&comp_controls_);

    eq_select_btn_.setRadioGroupId(Global::EQ_COMP_SELECT_GROUP);
    comp_select_btn_.setRadioGroupId(Global::EQ_COMP_SELECT_GROUP);

    eq_select_btn_.onClick   = [&]() { setView(&eq_select_btn_, 0); };
    comp_select_btn_.onClick = [&]() { setView(&comp_select_btn_, 1); };

    eq_select_btn_.setToggleState(true, juce::NotificationType::sendNotification);
}

/*---------------------------------------------------------------------------
**
*/
void
ControlsContainer::paint(juce::Graphics& g)
{
    //    g.fillAll(juce::Colours::blue);

    // Horizontal dividing line between the mode select buttons and the rotary controls.
    auto line_y = eq_select_btn_.getBottom();
    g.setColour(TailwindColours::Gray800);
    g.drawLine(0, line_y, getLocalBounds().getWidth(), line_y, 2.f);
}

/*---------------------------------------------------------------------------
**
*/
void
ControlsContainer::resized()
{
    auto bounds            = getLocalBounds();
    auto bounds_width      = bounds.getWidth();
    auto bounds_height     = bounds.getHeight();
    auto select_btn_height = 20;
    auto select_btn_width  = 100;
    auto power_btn_width   = select_btn_width * 0.3;
    auto power_btn_height  = select_btn_height - 4;
    auto button_padding    = (select_btn_height - power_btn_height) * 2;

    juce::Rectangle< int > power_btn_bounds(0, 0, power_btn_width, power_btn_height);

    eq_enabled_btn_.setBounds(power_btn_bounds.withX((select_btn_width * 0.5) - (power_btn_width * 0.5)));
    comp_enabled_btn_.setBounds(power_btn_bounds.withX((select_btn_width * 1.5) - (power_btn_width * 0.5)));

    eq_select_btn_.setBounds(0, eq_enabled_btn_.getBottom() + button_padding, select_btn_width, select_btn_height);
    comp_select_btn_.setBounds(select_btn_width,
                               comp_enabled_btn_.getBottom() + button_padding,
                               select_btn_width,
                               select_btn_height);

    juce::Rectangle< int > eq_bounds(0,
                                     eq_select_btn_.getBottom() + select_btn_height,
                                     bounds_width,
                                     bounds_height - (select_btn_height * 4));

    auto comp_width  = eq_bounds.getWidth() * 0.8;
    auto comp_height = eq_bounds.getHeight() * 0.8;
    juce::Rectangle< int > comp_bounds(bounds.getCentreX() - (comp_width * 0.5), eq_bounds.getY(), comp_width, comp_height);

    eq_controls_.setBounds(eq_bounds);
    comp_controls_.setBounds(comp_bounds);
}

/*---------------------------------------------------------------------------
**
*/
void
ControlsContainer::setView(juce::ToggleButton* button, uint8_t new_index)
{
    if ((button == nullptr) || !button->getToggleState()) {
        return;
    }

    eq_controls_.setVisible(new_index == 0);
    comp_controls_.setVisible(new_index == 1);
}

/*---------------------------------------------------------------------------
** End of File
*/
