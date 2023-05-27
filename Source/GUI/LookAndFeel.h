/*
  ==============================================================================

    LookAndFeel.h
    Created: 5 May 2023 2:46:08pm
    Author:  loren

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider(juce::Graphics&,
        int x, int y, int width, int height,
        float posicionProporcionalSlider,
        float anguloDeRotacionDeComienzo,
        float anguloDeRotacionDeFin,
        juce::Slider&) override;

    void drawToggleButton(juce::Graphics& g,
        juce::ToggleButton& toggleButton,
        bool dibujaBotonIluminado,
        bool dibujaBotonApagado) override;
};
