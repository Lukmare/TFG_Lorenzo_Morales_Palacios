/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/LookAndFeel.h"
#include "GUI/ControlesGlobales.h"
#include "GUI/ControlesDeCompresorDeBandas.h"
#include "GUI/ComponentesEstructurales.h"
#include "GUI/ComponenteParaConstruirOnda.h"
#include "GUI/BotonesPersonalizados.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_audio_processors/juce_audio_processors.h>


//***********************************************************************************************************************************

//==============================================================================

//***********************************************************************************************************************************

struct BarraControl : juce::Component
{
    BarraControl();
    void resized() override;

    BotonDelAnalizador botonAnalizador;
    BotonDeEncendido botonBypassGlobal;
};
//==============================================================================

class BandasDeSonidoCompuestoAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
{
public:
    BandasDeSonidoCompuestoAudioProcessorEditor (BandasDeSonidoCompuestoAudioProcessor&);
    ~BandasDeSonidoCompuestoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;
private:
    //************************************************************************************************************

    //*******************************************************************************************************
    LookAndFeel lnf;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BandasDeSonidoCompuestoAudioProcessor& audioProcessor;

    BarraControl control;
    ComponenteAnalizador analizador{audioProcessor};
    ControlesGlobales controlGlobal{audioProcessor.apvts};
    ControlesDeCompresorDeBandas controlBandas{ audioProcessor.apvts };

    void muestraEstadoDelBypassGlobal();

    std::array<juce::AudioParameterBool*, 3> getParametrosDelBypass();

    void actualizaBypassGlobal();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandasDeSonidoCompuestoAudioProcessorEditor)
};
