#pragma once

#include <JuceHeader.h>

//==============================================================================
class FFTBuffer
{
public:
    FFTBuffer();

    enum OrderBit { SIZE_1024 = 10, SIZE_2048 = 11, SIZE_4096 = 12 };

    static constexpr int                      FFT_ORDER = SIZE_2048;
    static constexpr int                      FFT_SIZE  = 1 << FFT_ORDER;
    typedef std::array< float, FFT_SIZE * 2 > FFTDataBlock;

    void prepare(double sample_rate);

    void pushNextSample(float sample);
    void getNextBlock(FFTDataBlock& block);

    double getBinWidth();

private:
    double                        sample_rate_;
    std::array< float, FFT_SIZE > ring_buffer_;
    size_t                        buffer_index_;
};
