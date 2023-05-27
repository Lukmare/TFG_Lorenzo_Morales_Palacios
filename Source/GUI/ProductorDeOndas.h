/*
  ==============================================================================

    ProductorDeOndas.h
    Created: 5 May 2023 8:44:49pm
    Author:  loren

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GeneradorDeDatosFFT.h"
#include "GeneradorDeOndaParaAnalizador.h"
#include "../PluginProcessor.h"

struct PathProducer
{
    PathProducer(SingleChannelSampleFifo<BandasDeSonidoCompuestoAudioProcessor::BlockType>& scsf) :
        leftChannelFifo(&scsf)
    {
        leftChannelFFTDataGenerator.changeOrder(FFTOrder::order2048);
        monoBuffer.setSize(1, leftChannelFFTDataGenerator.getFFTSize());
    }
    void process(juce::Rectangle<float> fftBounds, double sampleRate);
    juce::Path getPath() { return leftChannelFFTPath; }

    void actualizaNegativoInfinito(float nf) { negativoInfinito = nf; };
private:
    SingleChannelSampleFifo<BandasDeSonidoCompuestoAudioProcessor::BlockType>* leftChannelFifo;

    juce::AudioBuffer<float> monoBuffer;

    FFTDataGenerator<std::vector<float>> leftChannelFFTDataGenerator;

    AnalyzerPathGenerator<juce::Path> pathProducer;

    juce::Path leftChannelFFTPath;

    float negativoInfinito{ -48.f };

};
