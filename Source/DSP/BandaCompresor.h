/*
  ==============================================================================

    BandaCompresor.h
    Created: 5 May 2023 3:37:06pm
    Author:  loren

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../GUI/Funcionalidades.h"

struct BandaCompresor
{

    juce::AudioParameterChoice* ratio{ nullptr };
    juce::AudioParameterFloat* attack{ nullptr };
    juce::AudioParameterFloat* limite{ nullptr };
    juce::AudioParameterFloat* libera{ nullptr };
    juce::AudioParameterBool* bypass{ nullptr };
    juce::AudioParameterBool* mute{ nullptr };
    juce::AudioParameterBool* solo{ nullptr };
    void prepara(const juce::dsp::ProcessSpec& especificaciones);

    void actualizaOpcionesCompresor();
    void procesa(juce::AudioBuffer<float>& buffer);

    float getnivelRMSdbDeEntrada() const {return nivelRMSdbDeEntrada;}
    float getnivelRMSdbDeSalida() const { return nivelRMSdbDeSalida; }

private:

    juce::dsp::Compressor<float> compresor;

    std::atomic<float> nivelRMSdbDeEntrada{ INFINITO_NEGATIVO };
    std::atomic<float> nivelRMSdbDeSalida{ INFINITO_NEGATIVO };

    template<typename T>
    float compNivelRMS(const T& buffer)
    {
        int numeroCanales = static_cast<int>(buffer.getNumChannels());
        int numeroSamples = static_cast<int>(buffer.getNumSamples());

        auto rms = 0.f;
        for (int canal = 0; canal < numeroCanales; ++canal)
        {
            rms += buffer.getRMSLevel(canal, 0, numeroSamples);
        }

        rms /= static_cast<float>(numeroCanales);
        return rms;

    }
};