/*
  ==============================================================================

    ComponentesEstructurales.h
    Created: 5 May 2023 3:05:01pm
    Author:  loren

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


struct Estructura : juce::Component
{
    Estructura();
    void paint(juce::Graphics& x)override;

    juce::Colour color;
};

struct SliderRotatorio : juce::Slider
{
    SliderRotatorio();
};
