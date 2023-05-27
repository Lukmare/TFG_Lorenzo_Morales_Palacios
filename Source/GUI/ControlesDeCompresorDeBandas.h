/*
  ==============================================================================

    ControlesDeCompresorDeBandas.h
    Created: 5 May 2023 3:19:08pm
    Author:  loren

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SliderRotatorioConEtiquetas.h"

struct ControlesDeCompresorDeBandas : juce::Component, juce::Button::Listener
{
    ControlesDeCompresorDeBandas(juce::AudioProcessorValueTreeState& apvts);
    ~ControlesDeCompresorDeBandas() override;
    void resized() override;
    void paint(juce::Graphics&) override;

    void buttonClicked(juce::Button* boton) override;

    void muestraTodasLasBandas(bool bypassActivo);

private:
    juce::AudioProcessorValueTreeState& apvts;

    SliderRotatorioConEtiquetas sliderDeAttack, sliderDeLibera, sliderDeLimite;
    SliderRatio sliderDeRatio;
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> sliderDeAttackAttachment, sliderDeLimiteAttachment, sliderDeLiberaAttachment, sliderDeRatioAttachment;

    juce::ToggleButton botonBypass, botonSolo, botonMute, bandaBajo, bandaMedio, bandaAlto;
    using BotonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<BotonAttachment> botonBypassAttachment, botonSoloAttachment, botonMuteAttachment;

    juce::Component::SafePointer<ControlesDeCompresorDeBandas> punteroSeguro{ this };
    
    juce::ToggleButton* bandaActiva = &bandaBajo;

    void actualizaAttachments();
    void actualizaHabilitadoresDeSlider();
    void actualizaSMBEstados(juce::Button& botonActivado);
    void actualizaColoresDeBandaActiva(juce::Button& botonActivado);
    void resetColoresDeBandaActiva();
    static void refrescaColoresDeBandaActiva(juce::Button& banda, juce::Button& color);
    void actualizaEstadoSeleccionado();
};
