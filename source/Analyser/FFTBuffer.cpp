#include "FFTBuffer.h"

/*---------------------------------------------------------------------------
**
*/
FFTBuffer::FFTBuffer()
    : sample_rate_(0.0)
    , buffer_index_(0)
{
}

/*---------------------------------------------------------------------------
**
*/
void
FFTBuffer::prepare(double sample_rate)
{
    sample_rate_ = sample_rate;

    std::fill(ring_buffer_.begin(), ring_buffer_.end(), 0.f);
}

/*---------------------------------------------------------------------------
**
*/
void
FFTBuffer::pushNextSample(float sample)
{
    ring_buffer_.at(buffer_index_) = sample;

    ++buffer_index_;

    if (buffer_index_ == FFT_SIZE) {
        buffer_index_ = 0;
    }
}

/*---------------------------------------------------------------------------
**
*/
void
FFTBuffer::getNextBlock(FFTBuffer::FFTDataBlock& block)
{
    std::fill(block.begin(), block.end(), 0.f);
    std::copy(ring_buffer_.begin(), ring_buffer_.end(), block.begin());
}

/*---------------------------------------------------------------------------
**
*/
double
FFTBuffer::getBinWidth()
{
    return sample_rate_ / static_cast< double >(FFT_SIZE);
}

/*---------------------------------------------------------------------------
** End of File
*/
