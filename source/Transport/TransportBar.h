#pragma once

#include "JuceHeader.h"

#include "TransportButton.h"
#include "../PluginProcessor.h"

//==============================================================================
class TransportBar
    : public juce::Component
    , public juce::ChangeListener
{
public:
    TransportBar(DvaManagerProcessor& processor_ref);

    void paint(juce::Graphics& g) override;
    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    enum TRANSPORT_STATE { STOPPED, PLAYING, PAUSED };
    void updateTransportState(TRANSPORT_STATE new_state);

private:
    DvaManagerProcessor&        processor_ref_;
    juce::AudioTransportSource& transport_source_;
    TRANSPORT_STATE             transport_state_;

    void playButtonClicked();
    void stopButtonClicked();
    void loopButtonClicked();

    TransportButton play_button_;
    TransportButton stop_button_;
    TransportButton loop_button_;

    std::unique_ptr< juce::DrawableImage > play_icon_;
    std::unique_ptr< juce::DrawableImage > pause_icon_;
    std::unique_ptr< juce::DrawableImage > stop_icon_;
    std::unique_ptr< juce::DrawableImage > loop_icon_;
};
