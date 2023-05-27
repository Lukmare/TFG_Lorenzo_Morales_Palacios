/*
  ==============================================================================

    ComponenteParaConstruirOnda.h
    Created: 5 May 2023 8:45:18pm
    Author:  loren

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../DSP/BandaCompresor.h"
#include "ProductorDeOndas.h"

struct ComponenteAnalizador : juce::Component,
    juce::AudioProcessorParameter::Listener,
    juce::Timer
{
    ComponenteAnalizador(BandasDeSonidoCompuestoAudioProcessor&);
    ~ComponenteAnalizador();

    void parameterValueChanged(int parameterIndex, float newValue) override;

    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override { }

    void timerCallback() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void toggleAnalysisEnablement(bool enabled)
    {
        shouldShowFFTAnalysis = enabled;
    }

    void actualiza(const std::vector<float>& valores);

private:
    BandasDeSonidoCompuestoAudioProcessor& audioProcessor;

    bool shouldShowFFTAnalysis = true;

    juce::Atomic<bool> parametersChanged{ false };

   // void drawBackgroundGrid(juce::Graphics& g);
    void drawBackgroundGrid(juce::Graphics& g,juce::Rectangle<int> bounds);
    void drawTextLabels(juce::Graphics& g, juce::Rectangle<int> bounds);

    std::vector<float> getFrequencies();
    std::vector<float> getGains();
    std::vector<float> getXs(const std::vector<float>& freqs, float left, float width);

    juce::Rectangle<int> getRenderArea(juce::Rectangle<int> bounds);

    juce::Rectangle<int> getAnalysisArea(juce::Rectangle<int> bounds);

    PathProducer leftPathProducer, rightPathProducer;

    void dibujaAnalisisFFT(juce::Graphics& g, juce::Rectangle<int> bounds);
    void dibujaFondos(juce::Graphics& g, juce::Rectangle<int> bounds);
    
    juce::AudioParameterFloat* bajoParametroPtr{ nullptr };
    juce::AudioParameterFloat* altoParametroPtr{ nullptr };
    juce::AudioParameterFloat* bajoLimiteParametroPtr{ nullptr };
    juce::AudioParameterFloat* medioLimiteParametroPtr{ nullptr };
    juce::AudioParameterFloat* altoLimiteParametroPtr{ nullptr };
    
    float bandaBajoGR{ 0.f };
    float bandaMedioGR{ 0.f };
    float bandaAltoGR{ 0.f };

};
