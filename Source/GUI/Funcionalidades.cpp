/*
  ==============================================================================

    Funcionalidades.cpp
    Created: 5 May 2023 3:11:12pm
    Author:  loren

  ==============================================================================
*/

#include "Funcionalidades.h"


template<typename T>
bool truncadoKV(T& valor)
{
    if (valor > static_cast<T>(999))
    {
        valor /= static_cast<T>(1000);
        return true;
    }
    return false;
}

juce::String getValEtiquetas(const juce::RangedAudioParameter& parametro,
    bool getBajo,
    juce::String sufijo)
{
    juce::String str;

    auto val = getBajo ? parametro.getNormalisableRange().start :
        parametro.getNormalisableRange().end;

    bool valorK = truncadoKV(val);

    str << val;

    if (valorK)
    {
        str << "k";
    }

    str << sufijo;

    return str;
}


juce::Rectangle <int> dibujaFondo(juce::Graphics& x, juce::Rectangle<int> bounds) {
    using namespace juce;
    x.setColour(Colours::lightsteelblue);
    x.fillAll();

    auto boundsLocales = bounds;

    bounds.reduce(3, 3);
    x.setColour(Colours::black);
    x.fillRoundedRectangle(bounds.toFloat(), 3);

    x.drawRect(boundsLocales);

    return bounds;
}