/*
  ==============================================================================

    ComponentesEstructurales.cpp
    Created: 5 May 2023 3:05:01pm
    Author:  loren

  ==============================================================================
*/

#include "ComponentesEstructurales.h"

Estructura::Estructura()
{
    juce::Random r;
    color = juce::Colour(r.nextInt(150), r.nextInt(50), r.nextInt(200));
}

void Estructura::paint(juce::Graphics& x)
{
    x.fillAll(color);
}

SliderRotatorio::SliderRotatorio() :
    juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
        juce::Slider::TextEntryBoxPosition::NoTextBox)
{}