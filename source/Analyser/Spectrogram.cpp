#include "Spectrogram.h"
#include "../Utilities/GlobalConstants.h"

/*---------------------------------------------------------------------------
**
*/
Spectrogram::Spectrogram(FFTBuffer& fft_buffer)
    : fft_(FFTBuffer::FFT_ORDER)
    , windowing_function_(FFTBuffer::FFT_SIZE, juce::dsp::WindowingFunction< float >::blackmanHarris)
    , fft_buffer_(fft_buffer)
    , path_write_index_(1)
    , path_read_index_(0)
{
    std::fill(fft_data_.begin(), fft_data_.end(), 0.f);
    std::fill(paths_write_buffer_.begin(), paths_write_buffer_.end(), juce::Path());
    std::fill(path_read_buffer_.begin(), path_read_buffer_.end(), juce::Path());

    startTimerHz(60);
}

/*---------------------------------------------------------------------------
**
*/
Spectrogram::~Spectrogram()
{
    stopTimer();
}

/*---------------------------------------------------------------------------
**
*/
void
Spectrogram::paint(juce::Graphics& g)
{
    auto bounds         = getLocalBounds();
    auto line_thickness = 1.f;

    // Paths.

    // Oldest to newest (not including newest).
    for (size_t i = 0; i < NUM_PATHS - 1; ++i) {
        auto path = path_read_buffer_.at(i);

        if (path.isEmpty()) {
            continue;
        }

        auto delta = static_cast< float >(i + 1) / static_cast< float >(NUM_PATHS);

        g.setColour(path_colours_.at(i).withAlpha(0.3f));
        g.setOpacity(delta * 0.2);

        auto scaled_rect = bounds.withSizeKeepingCentre(bounds.getWidth() * delta, bounds.getHeight() * delta).toFloat();
        auto transform   = path.getTransformToScaleToFit(scaled_rect, true, juce::Justification::centred);

        g.strokePath(path, juce::PathStrokeType(line_thickness), transform);
    }

    // Newest.
    g.setColour(path_colours_.front());
    g.setOpacity(1.f);
    g.strokePath(path_read_buffer_.back(), juce::PathStrokeType(line_thickness));
}

/*---------------------------------------------------------------------------
**
*/
void
Spectrogram::timerCallback()
{
    processFFTData();
    generatePath();
    reorderReadPaths();
    repaint();
}

/*---------------------------------------------------------------------------
**
*/
void
Spectrogram::processFFTData()
{
    fft_buffer_.getNextBlock(fft_data_);

    windowing_function_.multiplyWithWindowingTable(fft_data_.data(), FFTBuffer::FFT_SIZE);

    fft_.performFrequencyOnlyForwardTransform(fft_data_.data(), true);

    int num_bins = std::floor(FFTBuffer::FFT_SIZE * 0.5);
    juce::FloatVectorOperations::multiply(fft_data_.data(), 1.f / static_cast< float >(num_bins), num_bins + 1);

    for (size_t i = 0; i < fft_data_.size(); ++i) {
        fft_data_.at(i) = juce::Decibels::gainToDecibels(fft_data_.at(i), Global::NEG_INF);
    }
}

/*---------------------------------------------------------------------------
**
*/
void
Spectrogram::generatePath()
{
    auto   bounds        = getLocalBounds();
    auto   bounds_x      = bounds.getX();
    auto   bounds_y      = bounds.getY();
    auto   bounds_width  = bounds.getWidth();
    auto   bounds_height = bounds.getHeight();
    size_t num_bins      = std::floor(FFTBuffer::FFT_SIZE * 0.5);
    float  bin_width     = static_cast< float >(fft_buffer_.getBinWidth());

    auto getYCoord = [&](const float& sample) -> float {
        return juce::jmap< float >(sample, Global::NEG_INF, Global::MAX_DB, bounds_height, bounds_y);
    };

    auto y_coord = getYCoord(fft_data_.at(0));

    if (std::isnan(y_coord) || std::isinf(y_coord)) {
        y_coord = bounds_height;
    }

    auto& path = paths_write_buffer_.at(path_write_index_);

    path.clear();
    path.startNewSubPath(bounds_x, bounds_height);

    const int path_resolution = 2;

    for (size_t i = 1; i < num_bins + 1; i += path_resolution) {
        y_coord = getYCoord(fft_data_.at(i));

        if (std::isnan(y_coord) || std::isinf(y_coord)) {
            continue;
        }

        auto freq            = i * bin_width;
        auto normalized_freq = juce::mapFromLog10(freq, Global::MIN_HZ, Global::MAX_HZ);
        auto x_coord         = std::floor(normalized_freq * bounds_width);

        if (x_coord > bounds_width) {
            continue;
        }

        path.lineTo(x_coord, y_coord);
    }

    path.lineTo(bounds_width, bounds_height);
    //    path.closeSubPath();

    // Increment write and read indexes.
    // Reset to zero if necessary.
    // Read index just trails 1 behind write index.

    if (++path_write_index_ == NUM_PATHS) {
        path_write_index_ = 0;
    }

    if (++path_read_index_ == NUM_PATHS) {
        path_read_index_ = 0;
    }
}

/*---------------------------------------------------------------------------
**
*/
void
Spectrogram::reorderReadPaths()
{
    // Copy the paths from the write ring buffer to the read buffer in order: oldest to newest.
    // Read index represents the last path written. i.e. the newest.
    // Read index + 1 is the oldest path in the write buffer.

    auto copy_offset = paths_write_buffer_.size() - path_read_index_;

    // From read index to end.
    std::copy(paths_write_buffer_.begin() + path_read_index_, paths_write_buffer_.end(), path_read_buffer_.begin());
    // From start to read index.
    std::copy(paths_write_buffer_.begin(),
              paths_write_buffer_.begin() + path_read_index_,
              path_read_buffer_.begin() + copy_offset);
}

/*---------------------------------------------------------------------------
** End of File
*/