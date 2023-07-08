#include "FilterDsp.h"

/*---------------------------------------------------------------------------
**
*/
void
FilterDsp::updatePeakCoefficients(MonoChain&                 mono_chain,
                                  const ChainPositions&      chain_position,
                                  juce::AudioParameterFloat* freq_param,
                                  juce::AudioParameterFloat* q_param,
                                  juce::AudioParameterFloat* gain_param,
                                  double                     sample_rate)
{
    if (freq_param == nullptr || q_param == nullptr || gain_param == nullptr) {
        return;
    }

    auto peak_coefficients = juce::dsp::IIR::Coefficients< float >::makePeakFilter(sample_rate,
                                                                                   freq_param->get(),
                                                                                   q_param->get(),
                                                                                   juce::Decibels::decibelsToGain(
                                                                                       gain_param->get()));

    switch (chain_position) {
    case ChainPositions::LF:
    {
        *mono_chain.get< ChainPositions::LF >().coefficients = *peak_coefficients;
        break;
    }
    case ChainPositions::LMF:
    {
        *mono_chain.get< ChainPositions::LMF >().coefficients = *peak_coefficients;
        break;
    }
    case ChainPositions::HMF:
    {
        *mono_chain.get< ChainPositions::HMF >().coefficients = *peak_coefficients;
        break;
    }
    case ChainPositions::HF:
    {
        *mono_chain.get< ChainPositions::HF >().coefficients = *peak_coefficients;
        break;
    }
    }
}

/*---------------------------------------------------------------------------
**
*/
void
FilterDsp::updateCutCoefficients(MonoChain&                  mono_chain,
                                 const ChainPositions&       chain_position,
                                 Coefficients&               coefficients,
                                 juce::AudioParameterChoice* slope_param)
{
    if (slope_param == nullptr) {
        return;
    }

    auto& cut_filter_chain = chain_position == ChainPositions::LOW_CUT ? mono_chain.get< ChainPositions::LOW_CUT >() :
                                                                         mono_chain.get< ChainPositions::HIGH_CUT >();
    cut_filter_chain.setBypassed< SLOPE_12 >(true);
    cut_filter_chain.setBypassed< SLOPE_24 >(true);
    cut_filter_chain.setBypassed< SLOPE_36 >(true);
    cut_filter_chain.setBypassed< SLOPE_48 >(true);

    switch (slope_param->getIndex()) {
    case SLOPE_48:
    {
        *cut_filter_chain.template get< SLOPE_48 >().coefficients = *coefficients[SLOPE_48];
        cut_filter_chain.setBypassed< SLOPE_48 >(false);
    }
    case SLOPE_36:
    {
        *cut_filter_chain.template get< SLOPE_36 >().coefficients = *coefficients[SLOPE_36];
        cut_filter_chain.setBypassed< SLOPE_36 >(false);
    }
    case SLOPE_24:
    {
        *cut_filter_chain.template get< SLOPE_24 >().coefficients = *coefficients[SLOPE_24];
        cut_filter_chain.setBypassed< SLOPE_24 >(false);
    }
    case SLOPE_12:
    {
        *cut_filter_chain.template get< SLOPE_12 >().coefficients = *coefficients[SLOPE_12];
        cut_filter_chain.setBypassed< SLOPE_12 >(false);
        break;
    }
    default:
    {
        break;
    }
    }
}

/*---------------------------------------------------------------------------
**
*/
FilterDsp::Coefficients
FilterDsp::makeHighPassFilter(juce::AudioParameterFloat*  freq_param,
                              juce::AudioParameterChoice* slope_param,
                              double                      sample_rate)
{
    // TODO nullptr checks (what should be returned by default for FilterUtils::CoefficientsType?)
    return juce::dsp::FilterDesign< float >::designIIRHighpassHighOrderButterworthMethod(freq_param->get(),
                                                                                         sample_rate,
                                                                                         (slope_param->getIndex() + 1)
                                                                                             * 2);
}

/*---------------------------------------------------------------------------
**
*/
FilterDsp::Coefficients
FilterDsp::makeLowPassFilter(juce::AudioParameterFloat*  freq_param,
                             juce::AudioParameterChoice* slope_param,
                             double                      sample_rate)
{
    // TODO nullptr checks (what should be returned by default for FilterUtils::CoefficientsType?)
    return juce::dsp::FilterDesign< float >::designIIRLowpassHighOrderButterworthMethod(freq_param->get(),
                                                                                        sample_rate,
                                                                                        (slope_param->getIndex() + 1) * 2);
}
#if 0
/*---------------------------------------------------------------------------
**
*/
void
FilterDsp::updateBandEnablements(MonoChain&                                          monoChain,
                                 juce::AudioParameterBool*                           lcEnabledParam,
                                 juce::AudioParameterBool*                           hcEnabledParam,
                                 std::array< PeakBand, Globals::getNumPeakBands() >& peakBands)
{
    if (lcEnabledParam == nullptr || hcEnabledParam == nullptr) {
        return;
    }

    for (size_t i = 0; i < Globals::getNumPeakBands(); ++i) {
        if (peakBands.at(i).peakEnabledParam == nullptr) {
            return;
        }
    }

    monoChain.setBypassed< FilterDsp::ChainPositions::LOW_CUT >(!lcEnabledParam->get());
    monoChain.setBypassed< FilterDsp::ChainPositions::HIGH_CUT >(!hcEnabledParam->get());
    monoChain.setBypassed< FilterDsp::ChainPositions::LF >(!peakBands.at(0).peakEnabledParam->get());
    monoChain.setBypassed< FilterDsp::ChainPositions::LMF >(!peakBands.at(1).peakEnabledParam->get());
    monoChain.setBypassed< FilterDsp::ChainPositions::HMF >(!peakBands.at(2).peakEnabledParam->get());
    monoChain.setBypassed< FilterDsp::ChainPositions::Peak_3 >(!peakBands.at(3).peakEnabledParam->get());
    monoChain.setBypassed< FilterDsp::ChainPositions::Peak_4 >(!peakBands.at(4).peakEnabledParam->get());
    monoChain.setBypassed< FilterDsp::ChainPositions::Peak_5 >(!peakBands.at(5).peakEnabledParam->get());
}
#endif
/*---------------------------------------------------------------------------
** End of File
*/
