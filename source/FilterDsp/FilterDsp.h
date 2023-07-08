#pragma once

#include "JuceHeader.h"

//==============================================================================
class FilterDsp
{
public:
    using Filter = juce::dsp::IIR::Filter< float >;
    // Cut Filter chain: x4 Filters to allow for 12/24/36/48 dB/oct options
    using CutFilter    = juce::dsp::ProcessorChain< Filter, Filter, Filter, Filter >;
    using MonoChain    = juce::dsp::ProcessorChain< CutFilter, Filter, Filter, Filter, Filter, CutFilter >;
    using Coefficients = juce::ReferenceCountedArray< juce::dsp::FilterDesign< float >::IIRCoefficients >;

    enum ChainPositions { LOW_CUT, LF, LMF, HMF, HF, HIGH_CUT };

    enum Slope { SLOPE_12, SLOPE_24, SLOPE_36, SLOPE_48 };

    static void updatePeakCoefficients(MonoChain&                 mono_chain,
                                       const ChainPositions&      chain_position,
                                       juce::AudioParameterFloat* freq_param,
                                       juce::AudioParameterFloat* q_param,
                                       juce::AudioParameterFloat* gain_param,
                                       double                     sample_rate);

    static void updateCutCoefficients(MonoChain&                  mono_chain,
                                      const ChainPositions&       chain_position,
                                      Coefficients&               coefficients,
                                      juce::AudioParameterChoice* slope_param);

    static Coefficients
    makeHighPassFilter(juce::AudioParameterFloat* freq_param, juce::AudioParameterChoice* slope_param, double sample_rate);

    static Coefficients
    makeLowPassFilter(juce::AudioParameterFloat* freq_param, juce::AudioParameterChoice* slope_param, double sample_rate);
#if 0
    static void updateBandEnablements(MonoChain&                                          monoChain,
                                      juce::AudioParameterBool*                           lcEnabledParam,
                                      juce::AudioParameterBool*                           hcEnabledParam,
                                      std::array< PeakBand, Globals::getNumPeakBands() >& peakBands);
#endif
};
