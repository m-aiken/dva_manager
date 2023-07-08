#pragma once

#include <JuceHeader.h>

//==============================================================================
class LufsFilter
{
public:
    LufsFilter();

    void  prepare(const juce::dsp::ProcessSpec& spec);
    float getMonoLufs(juce::AudioBuffer< float >& buffer);

private:
    juce::dsp::IIR::Filter< float >            shelf_filter_;
    juce::dsp::IIR::Filter< float >            highpass_filter_;
    juce::dsp::IIR::Coefficients< float >::Ptr shelf_coefficients_;
    juce::dsp::IIR::Coefficients< float >::Ptr highpass_coefficients_;

    // We copy the incoming buffer to perform our calculations on so that the audio output isn't affected.
    juce::AudioBuffer< float > local_audio_buffer_;
};
