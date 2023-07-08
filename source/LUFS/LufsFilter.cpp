#include "LufsFilter.h"

/*---------------------------------------------------------------------------
**
*/
LufsFilter::LufsFilter()
{
    // Initialise the filter coefficients.
    float shelf_coeff_b0 = 1.53512485958697f;
    float shelf_coeff_b1 = -2.69169618940638f;
    float shelf_coeff_b2 = 1.19839281085285f;
    float shelf_coeff_a0 = 1.f;
    float shelf_coeff_a1 = -1.69065929318241f;
    float shelf_coeff_a2 = 0.73248077421585f;

    shelf_coefficients_ = new juce::dsp::IIR::Coefficients< float >(shelf_coeff_b0,
                                                                    shelf_coeff_b1,
                                                                    shelf_coeff_b2,
                                                                    shelf_coeff_a0,
                                                                    shelf_coeff_a1,
                                                                    shelf_coeff_a2);

    float highpass_coeff_b0 = 1.0f;
    float highpass_coeff_b1 = -2.0f;
    float highpass_coeff_b2 = 1.0f;
    float highpass_coeff_a0 = 1.f;
    float highpass_coeff_a1 = -1.99004745483398f;
    float highpass_coeff_a2 = 0.99007225036621f;

    highpass_coefficients_ = new juce::dsp::IIR::Coefficients< float >(highpass_coeff_b0,
                                                                       highpass_coeff_b1,
                                                                       highpass_coeff_b2,
                                                                       highpass_coeff_a0,
                                                                       highpass_coeff_a1,
                                                                       highpass_coeff_a2);
}

/*---------------------------------------------------------------------------
**
*/
void
LufsFilter::prepare(const juce::dsp::ProcessSpec& spec)
{
    shelf_filter_.prepare(spec);
    highpass_filter_.prepare(spec);
}

/*---------------------------------------------------------------------------
**
*/
float
LufsFilter::getMonoLufs(juce::AudioBuffer< float >& buffer)
{
    local_audio_buffer_.makeCopyOf(buffer);

    // Process Context.
    juce::dsp::AudioBlock< float >              audio_block(local_audio_buffer_);
    juce::dsp::ProcessContextReplacing< float > context(audio_block);

    //    auto test_rms = buffer.getRMSLevel(0, 0, buffer.getNumSamples());

    // Pre filtering.
    // Pre filtering - Stage 1.
    *shelf_filter_.coefficients = *shelf_coefficients_;
    shelf_filter_.process(context);

    //    test_rms = buffer.getRMSLevel(0, 0, buffer.getNumSamples());

    // Pre filtering - Stage 2.
    *highpass_filter_.coefficients = *highpass_coefficients_;
    highpass_filter_.process(context);

    //    test_rms = buffer.getRMSLevel(0, 0, buffer.getNumSamples());

    // Mean square.
    float sum = 0.f;

    for (auto i = 0; i < local_audio_buffer_.getNumSamples(); ++i) {
        sum += local_audio_buffer_.getSample(0, i);
    }

    float mean_square = std::pow(sum / local_audio_buffer_.getNumSamples(), 2);

    // Weighting.
    float mono_weight = 1.f;

    float weighted_mean_square = mean_square * mono_weight;

    // Loudness.
    float loudness = -0.691f + (10 * std::log10(weighted_mean_square));

    return loudness;
    //    return 0.f;
}

/*---------------------------------------------------------------------------
** End of File
*/
