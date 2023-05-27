/*
  ==============================================================================

    BandaCompresor.cpp
    Created: 5 May 2023 3:37:06pm
    Author:  loren

  ==============================================================================
*/

#include "BandaCompresor.h"

void BandaCompresor::prepara(const juce::dsp::ProcessSpec& especificaciones)
{
    compresor.prepare(especificaciones);
}

void BandaCompresor::actualizaOpcionesCompresor()
{
    compresor.setAttack(attack->get());
    compresor.setRelease(libera->get());
    compresor.setThreshold(limite->get());
    compresor.setRatio(ratio->getCurrentChoiceName().getFloatValue());

}

void BandaCompresor::procesa(juce::AudioBuffer<float>& buffer)
{

    auto preRMS = compNivelRMS(buffer);
    auto bloque = juce::dsp::AudioBlock<float>(buffer);
    auto contexto = juce::dsp::ProcessContextReplacing<float>(bloque);

    contexto.isBypassed = bypass->get();

    compresor.process(contexto);

    auto postRMS = compNivelRMS(buffer);

    auto convierteAdb = [](auto entrada)
    {
        return juce::Decibels::gainToDecibels(entrada);
    };

    nivelRMSdbDeEntrada.store(convierteAdb(preRMS));
    nivelRMSdbDeSalida.store(convierteAdb(postRMS));

}