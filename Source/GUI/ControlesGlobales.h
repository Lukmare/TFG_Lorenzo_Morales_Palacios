/*
  ==============================================================================

    ControlesGlobales.h
    Created: 5 May 2023 3:26:46pm
    Author:  loren

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SliderRotatorioConEtiquetas.h"

struct ControlesGlobales : juce::Component
{
    ControlesGlobales(juce::AudioProcessorValueTreeState& apvts);

    void paint(juce::Graphics& x) override;

    void resized() override;



private:
    using SRCE = SliderRotatorioConEtiquetas; // Slider Rotatorio Con Etiquetas
    juce::Slider volumeSlider;
    std::unique_ptr<SRCE> inGainSlider, bajoSlider, altoSlider, outGainSlider;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> bajoSliderAttachment,
        altoSliderAttachment,
        inGainSliderAttachment,
        outGainSliderAttachment;
};
