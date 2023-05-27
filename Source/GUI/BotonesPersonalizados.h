/*
  ==============================================================================

    BotonesPersonalizados.h
    Created: 5 May 2023 3:02:00pm
    Author:  loren

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct BotonDeEncendido : juce::ToggleButton { };

struct BotonDelAnalizador : juce::ToggleButton
{
    void resized();

    juce::Path pathAleatorio;
};
