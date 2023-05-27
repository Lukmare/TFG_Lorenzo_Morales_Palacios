/*
  ==============================================================================

    ControlesGlobales.cpp
    Created: 5 May 2023 3:26:46pm
    Author:  loren

  ==============================================================================
*/

#include "ControlesGlobales.h"
#include "../DSP/Parametros.h"
#include "Funcionalidades.h"
#include "ComponentesEstructurales.h"


ControlesGlobales::ControlesGlobales(juce::AudioProcessorValueTreeState& apvts)
{
    using namespace Parametros;
    const auto& parametros = GetParametros();

    auto getParametroHelper = [&parametros, &apvts](const auto& nombre)-> auto&
    {
        return getParametro(apvts, parametros, nombre);
    };

    auto& parametroGainEntrada = getParametroHelper(Nombres::Gain_Entrada);
    auto& parametroGainSalida = getParametroHelper(Nombres::Gain_Salida);
    auto& parametroMedioBajo = getParametroHelper(Nombres::Medio_Bajo_Freq_Tras);
    auto& parametroMedioAlto = getParametroHelper(Nombres::Medio_Alto_Freq_Tras);


    inGainSlider = std::make_unique<SRCE>(&parametroGainEntrada, "dB", "AJUSTE DE ENTRADA");
    outGainSlider = std::make_unique<SRCE>(&parametroGainSalida, "dB", "AJUSTE DE SALIDA");
    bajoSlider = std::make_unique<SRCE>(&parametroMedioBajo, "Hz", "MEDIO BAJO");
    altoSlider = std::make_unique<SRCE>(&parametroMedioAlto, "Hz", "MEDIO ALTO");

    auto creaAttachmentHelper = [&parametros, &apvts](auto& attachment, const auto& nombre, auto& slider)
    {
        creaAttachment(attachment, apvts, parametros, nombre, slider);
    };


    creaAttachmentHelper(inGainSliderAttachment,
        Nombres::Gain_Entrada,
        *inGainSlider);

    creaAttachmentHelper(bajoSliderAttachment,
        Nombres::Medio_Bajo_Freq_Tras,
        *bajoSlider);

    creaAttachmentHelper(altoSliderAttachment,
        Nombres::Medio_Alto_Freq_Tras,
        *altoSlider);

    creaAttachmentHelper(outGainSliderAttachment,
        Nombres::Gain_Salida,
        *outGainSlider);

    añadeEtiquetas(inGainSlider->etiquetas, getParametroHelper(Nombres::Gain_Entrada), "dB");
    añadeEtiquetas(outGainSlider->etiquetas, getParametroHelper(Nombres::Gain_Salida), "dB");
    añadeEtiquetas(altoSlider->etiquetas, getParametroHelper(Nombres::Medio_Alto_Freq_Tras), "Hz");
    añadeEtiquetas(bajoSlider->etiquetas, getParametroHelper(Nombres::Medio_Bajo_Freq_Tras), "Hz");


    addAndMakeVisible(*inGainSlider);
    addAndMakeVisible(*bajoSlider);
    addAndMakeVisible(*altoSlider);
    addAndMakeVisible(*outGainSlider);
}

void ControlesGlobales::paint(juce::Graphics& x)
{
    auto bounds = getLocalBounds();
    dibujaFondo(x, bounds);
}

void ControlesGlobales::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    using namespace juce;

    FlexBox flexB;
    flexB.flexDirection = FlexBox::Direction::row;
    flexB.flexWrap = FlexBox::Wrap::noWrap;

    auto espaciado = FlexItem().withWidth(4);
    auto finCap = FlexItem().withWidth(6);

    flexB.items.add(finCap);
    flexB.items.add(espaciado);
    flexB.items.add(FlexItem(*inGainSlider).withFlex(1.f));
    flexB.items.add(espaciado);
    flexB.items.add(FlexItem(*bajoSlider).withFlex(1.f));
    flexB.items.add(espaciado);
    flexB.items.add(FlexItem(*altoSlider).withFlex(1.f));
    flexB.items.add(espaciado);
    flexB.items.add(FlexItem(*outGainSlider).withFlex(1.f));
    flexB.items.add(finCap);

    flexB.performLayout(bounds);

}