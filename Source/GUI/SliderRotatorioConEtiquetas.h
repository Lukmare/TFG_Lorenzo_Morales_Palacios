/*
  ==============================================================================

    SliderRotatorioConEtiquetas.h
    Created: 5 May 2023 2:54:08pm
    Author:  loren

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


struct SliderRotatorioConEtiquetas : juce::Slider
{
    SliderRotatorioConEtiquetas(juce::RangedAudioParameter* rap, const juce::String& unitSuffix, const juce::String& titulo /*= "NO TITLE"*/) :
        juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
            juce::Slider::TextEntryBoxPosition::NoTextBox),
        parametro(rap),
        sufijo(unitSuffix)
    {
        setName(titulo);
        //      setLookAndFeel(&lnf);
    }

    /*  ~SliderRotatorioConEtiquetas()
      {
          setLookAndFeel(nullptr);
      }*/

    struct PosicionEtiqueta
    {
        float pos;
        juce::String etiqueta;
    };

    juce::Array<PosicionEtiqueta> etiquetas;

    void paint(juce::Graphics& g) override;
    juce::Rectangle<int> getSliderBounds() const;
    int getTextHeight() const { return 14; }
    virtual juce::String getDisplayString() const;

    void cambiaParametro(juce::RangedAudioParameter* p);
protected:
    juce::RangedAudioParameter* parametro;
    juce::String sufijo;
};

struct SliderRatio : SliderRotatorioConEtiquetas
{
    SliderRatio(juce::RangedAudioParameter* rap,
        const juce::String& unitSuffix) : SliderRotatorioConEtiquetas(rap, unitSuffix, "RATIO") {}
    juce::String getDisplayString() const override;

};