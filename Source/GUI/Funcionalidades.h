/*
  ==============================================================================

    Funcionalidades.h
    Created: 5 May 2023 3:11:12pm
    Author:  loren

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define FRECUENCIA_MINIMA 20.f
#define FRECUENCIA_MAXIMA 20000.f

#define INFINITO_NEGATIVO -72.f
#define DECIBELIOS_MAXIMO 12.f

#define LIMITE_MINIMO -60.f
enum FFTOrder
{
    order2048 = 11,
    order4096 = 12,
    order8192 = 13
};

template< typename Attachment, typename APVTS, typename Parametros, typename NombreParametro, typename TipoSlider>
void creaAttachment(std::unique_ptr<Attachment>& attachment, APVTS& apvts,
    const Parametros& parametros, const NombreParametro& nombre, TipoSlider& slider)
{
    attachment = std::make_unique<Attachment>(apvts,
        parametros.at(nombre),
        slider);
}

template<typename APVTS, typename Parametros, typename Nombre>
juce::RangedAudioParameter& getParametro(APVTS& apvts, const Parametros& parametros, const Nombre& nombre)
{
    auto parametro = apvts.getParameter(parametros.at(nombre));
    jassert(parametro != nullptr);

    return *parametro;

}
juce::Rectangle<int> dibujaFondo(juce::Graphics& x, juce::Rectangle<int> bounds);
 
template<typename T>
bool truncadoKV(T& valor);

juce::String getValEtiquetas(const juce::RangedAudioParameter& parametro,
    bool getBajo,
    juce::String sufijo);

template<typename Etiquetas, typename TipoParametro, typename TipoSufijo>
void añadeEtiquetas(Etiquetas& etiquetas, const TipoParametro& parametro, const TipoSufijo& sufijo)
{
    etiquetas.clear();
    etiquetas.add({ 0.f,getValEtiquetas(parametro,true,sufijo) });
    etiquetas.add({ 1.f,getValEtiquetas(parametro,false,sufijo) });

}

