#pragma once

#include <JuceHeader.h>

#include "FFTBuffer.h"
#include "../Utilities/TailwindColours.h"

//==============================================================================
class Spectrogram
    : public juce::Component
    , public juce::Timer
{
public:
    Spectrogram(FFTBuffer& fft_buffer);
    ~Spectrogram();

    void paint(juce::Graphics& g) override;

    void timerCallback() override;

    void processFFTData();
    void generatePath();

private:
    juce::dsp::FFT                        fft_;
    FFTBuffer::FFTDataBlock               fft_data_;
    juce::dsp::WindowingFunction< float > windowing_function_;
    FFTBuffer&                            fft_buffer_;

    static const int                    NUM_PATHS = 48;
    std::array< juce::Path, NUM_PATHS > paths_write_buffer_;
    size_t                              path_write_index_;
    std::array< juce::Path, NUM_PATHS > path_read_buffer_;
    size_t                              path_read_index_;

    void reorderReadPaths();

    std::array< juce::Colour, NUM_PATHS > path_colours_ {
        TailwindColours::Emerald300, TailwindColours::Emerald300, TailwindColours::Emerald300,
        TailwindColours::Emerald300, TailwindColours::Emerald300, TailwindColours::Emerald300,
        TailwindColours::Emerald300, TailwindColours::Emerald300, TailwindColours::Emerald400,
        TailwindColours::Emerald400, TailwindColours::Emerald400, TailwindColours::Emerald400,
        TailwindColours::Emerald400, TailwindColours::Emerald400, TailwindColours::Emerald400,
        TailwindColours::Emerald400, TailwindColours::Emerald500, TailwindColours::Emerald500,
        TailwindColours::Emerald500, TailwindColours::Emerald500, TailwindColours::Emerald500,
        TailwindColours::Emerald500, TailwindColours::Emerald500, TailwindColours::Emerald500,
        TailwindColours::Emerald600, TailwindColours::Emerald600, TailwindColours::Emerald600,
        TailwindColours::Emerald600, TailwindColours::Emerald600, TailwindColours::Emerald600,
        TailwindColours::Emerald600, TailwindColours::Emerald600, TailwindColours::Emerald700,
        TailwindColours::Emerald700, TailwindColours::Emerald700, TailwindColours::Emerald700,
        TailwindColours::Emerald700, TailwindColours::Emerald700, TailwindColours::Emerald700,
        TailwindColours::Emerald700, TailwindColours::Emerald800, TailwindColours::Emerald800,
        TailwindColours::Emerald800, TailwindColours::Emerald800, TailwindColours::Emerald800,
        TailwindColours::Emerald800, TailwindColours::Emerald800, TailwindColours::Emerald800
    };
};
