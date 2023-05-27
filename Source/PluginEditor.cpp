/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSP/Parametros.h"
#include <juce_gui_extra/juce_gui_extra.h>

BarraControl::BarraControl()
{
    botonAnalizador.setToggleState(true, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(botonAnalizador);
    addAndMakeVisible(botonBypassGlobal);
}

void BarraControl::resized()
{
    auto bounds = getLocalBounds();

    botonAnalizador.setBounds(bounds.removeFromLeft(50).withTrimmedBottom(4).withTrimmedTop(4));
    botonBypassGlobal.setBounds(bounds.removeFromRight(60).withTrimmedBottom(2).withTrimmedTop(2));
}
//==============================================================================
// 
// 
//
// 
//==============================================================================

//***********************************************************************************************************************************

//==============================================================================

//***********************************************************************************************************************************
BandasDeSonidoCompuestoAudioProcessorEditor::BandasDeSonidoCompuestoAudioProcessorEditor (BandasDeSonidoCompuestoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //***************************************************************************************************************

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setLookAndFeel(&lnf);

    control.botonAnalizador.onClick = [this]()
    {
        auto estaActivado = control.botonAnalizador.getToggleState();
        analizador.toggleAnalysisEnablement(estaActivado);
    };

    control.botonBypassGlobal.onClick = [this]()
    {
        muestraEstadoDelBypassGlobal();
    };

    addAndMakeVisible(control);
    addAndMakeVisible(analizador);
    addAndMakeVisible(controlGlobal);
    addAndMakeVisible(controlBandas);

    setSize (800, 600);

    startTimerHz(60);
}

BandasDeSonidoCompuestoAudioProcessorEditor::~BandasDeSonidoCompuestoAudioProcessorEditor()
{
    //************************************************************************

    //**************************************************************************************
    setLookAndFeel(nullptr);
}

//==============================================================================
void BandasDeSonidoCompuestoAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
   // g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    g.fillAll(juce::Colours::black);
}

void BandasDeSonidoCompuestoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    //LMP
    auto bounds = getLocalBounds();
    
   control.setBounds(bounds.removeFromTop(45));

    controlBandas.setBounds(bounds.removeFromBottom(155));

    analizador.setBounds(bounds.removeFromTop(255));

    controlGlobal.setBounds(bounds);

    //****************************************************************************************************************

    //******************************************************************************************************************
}

void BandasDeSonidoCompuestoAudioProcessorEditor::timerCallback() {

    std::vector<float> valores
    {
        audioProcessor.bandaCompresorBajo.getnivelRMSdbDeEntrada(),
        audioProcessor.bandaCompresorBajo.getnivelRMSdbDeSalida(),
        audioProcessor.bandaCompresorMedio.getnivelRMSdbDeEntrada(),
        audioProcessor.bandaCompresorMedio.getnivelRMSdbDeSalida(),
        audioProcessor.bandaCompresorAlto.getnivelRMSdbDeEntrada(),
        audioProcessor.bandaCompresorAlto.getnivelRMSdbDeSalida()
    };

    analizador.actualiza(valores);

    actualizaBypassGlobal();
}
void BandasDeSonidoCompuestoAudioProcessorEditor::actualizaBypassGlobal()
{
    auto parametros = getParametrosDelBypass();

    bool bandaGlobalEnBypass = std::all_of(parametros.begin(), parametros.end(), [](const auto& parametro) { return parametro->get(); });

    control.botonBypassGlobal.setToggleState(bandaGlobalEnBypass,
        juce::NotificationType::dontSendNotification);
}
void BandasDeSonidoCompuestoAudioProcessorEditor::muestraEstadoDelBypassGlobal()
{
    auto todoActivado = ! control.botonBypassGlobal.getToggleState();

    auto parametros = getParametrosDelBypass();

    auto bypassParametroHelper = [](auto* parametro, bool bypassActivo)
    {
        parametro->beginChangeGesture();
        parametro->setValueNotifyingHost(bypassActivo ? 1.f : 0.f);
        parametro->endChangeGesture();
    };

    for (auto* parametro : parametros)
    {
        bypassParametroHelper(parametro, ! todoActivado);
    }

    controlBandas.muestraTodasLasBandas(!todoActivado);
}

std::array<juce::AudioParameterBool*, 3> BandasDeSonidoCompuestoAudioProcessorEditor::getParametrosDelBypass()
{
    using namespace Parametros;
    using namespace juce;
    const auto& parametros = Parametros::GetParametros();
    auto& apvts = audioProcessor.apvts;


    auto boolHelper = [&apvts, &parametros](const auto& nombreParametro) {

        auto parametro = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(parametros.at(nombreParametro)));
        jassert(parametro != nullptr);

        return parametro;

    };

    auto* parametroBypassBajo = boolHelper(Nombres::Bypass_Banda_Bajo);
    auto* parametroBypassMedio = boolHelper(Nombres::Bypass_Banda_Medio);
    auto* parametroBypassAlto = boolHelper(Nombres::Bypass_Banda_Alto);

    return { parametroBypassBajo,parametroBypassMedio ,parametroBypassAlto };

}
//***********************************************************************************************

//*****************************************************************************************
