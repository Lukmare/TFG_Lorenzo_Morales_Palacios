/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSP/Parametros.h"
#include <juce_gui_extra/juce_gui_extra.h>

//*****************************************************************************************

//********************************************************************************************
//==============================================================================
BandasDeSonidoCompuestoAudioProcessor::BandasDeSonidoCompuestoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    //LMP

    using namespace Parametros;
    const auto& parametros = GetParametros(); 

    auto floatHelper = [&apvts = this-> apvts, &parametros](auto& parametro, const auto& nombreParametro) {

        parametro = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(parametros.at(nombreParametro)));
        jassert(parametro != nullptr);

    };

    floatHelper(bandaCompresorBajo.attack, Nombres::Attack_Banda_Bajo);
    floatHelper(bandaCompresorBajo.libera, Nombres::Libera_Banda_Bajo);
    floatHelper(bandaCompresorBajo.limite, Nombres::Limite_Banda_Bajo);

    floatHelper(bandaCompresorMedio.attack, Nombres::Attack_Banda_Medio);
    floatHelper(bandaCompresorMedio.libera, Nombres::Libera_Banda_Medio);
    floatHelper(bandaCompresorMedio.limite, Nombres::Limite_Banda_Medio);

    floatHelper(bandaCompresorAlto.attack, Nombres::Attack_Banda_Alto);
    floatHelper(bandaCompresorAlto.libera, Nombres::Libera_Banda_Alto);
    floatHelper(bandaCompresorAlto.limite, Nombres::Limite_Banda_Alto);

    auto choiceHelper = [&apvts = this-> apvts, &parametros](auto& parametro, const auto& nombreParametro) {

        parametro = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(parametros.at(nombreParametro)));
        jassert(parametro != nullptr);

    };

    choiceHelper(bandaCompresorBajo.ratio, Nombres::Ratio_Banda_Bajo);
    choiceHelper(bandaCompresorMedio.ratio, Nombres::Ratio_Banda_Medio);
    choiceHelper(bandaCompresorAlto.ratio, Nombres::Ratio_Banda_Alto);

    auto boolHelper = [&apvts = this-> apvts, &parametros](auto& parametro, const auto& nombreParametro) {
        
        parametro = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(parametros.at(nombreParametro)));
        jassert(parametro != nullptr);

    };

    boolHelper(bandaCompresorBajo.bypass, Nombres::Bypass_Banda_Bajo);
    boolHelper(bandaCompresorMedio.bypass, Nombres::Bypass_Banda_Medio);
    boolHelper(bandaCompresorAlto.bypass, Nombres::Bypass_Banda_Alto);


    boolHelper(bandaCompresorBajo.mute, Nombres::Mute_Banda_Bajo);
    boolHelper(bandaCompresorMedio.mute, Nombres::Mute_Banda_Medio);
    boolHelper(bandaCompresorAlto.mute, Nombres::Mute_Banda_Alto);


    boolHelper(bandaCompresorBajo.solo, Nombres::Solo_Banda_Bajo);
    boolHelper(bandaCompresorMedio.solo, Nombres::Solo_Banda_Medio);
    boolHelper(bandaCompresorAlto.solo, Nombres::Solo_Banda_Alto);

    floatHelper(parametroGainDeSalida, Nombres::Gain_Salida);
    floatHelper(parametroGainDeSalida, Nombres::Gain_Salida);

    floatHelper(bajoMedio, Nombres::Medio_Bajo_Freq_Tras);
    floatHelper(altoMedio, Nombres::Medio_Alto_Freq_Tras);

    floatHelper(parametroGainDeEntrada, Nombres::Gain_Entrada);
    floatHelper(parametroGainDeSalida, Nombres::Gain_Salida);

    BP1.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    AP1.setType(juce::dsp::LinkwitzRileyFilterType::highpass);

    FULLP2.setType(juce::dsp::LinkwitzRileyFilterType::allpass);

    BP2.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    AP2.setType(juce::dsp::LinkwitzRileyFilterType::highpass);

   // invFULLP1.setType(juce::dsp::LinkwitzRileyFilterType::allpass);

    //invFULLP2.setType(juce::dsp::LinkwitzRileyFilterType::allpass);

}
BandasDeSonidoCompuestoAudioProcessor::~BandasDeSonidoCompuestoAudioProcessor()
{
}

//==============================================================================
const juce::String BandasDeSonidoCompuestoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BandasDeSonidoCompuestoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BandasDeSonidoCompuestoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BandasDeSonidoCompuestoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BandasDeSonidoCompuestoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BandasDeSonidoCompuestoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BandasDeSonidoCompuestoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BandasDeSonidoCompuestoAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BandasDeSonidoCompuestoAudioProcessor::getProgramName (int index)
{
    return {};
}

void BandasDeSonidoCompuestoAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BandasDeSonidoCompuestoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    //LMP
    //********************************************************************************************************************************
    //*******************************************************************************************************************************
    juce::dsp::ProcessSpec especificaciones;
    especificaciones.maximumBlockSize = samplesPerBlock;
    especificaciones.numChannels = getTotalNumOutputChannels();
    especificaciones.sampleRate = sampleRate;

    for (auto& comp : compresores) 
    {
        comp.prepara(especificaciones);
    }
    

    BP1.prepare(especificaciones);
    AP1.prepare(especificaciones);

    FULLP2.prepare(especificaciones);

    BP2.prepare(especificaciones);
    AP2.prepare(especificaciones);

   // invFULLP1.prepare(especificaciones);
   // invFULLP2.prepare(especificaciones);

   // invFULLBuffer.setSize(especificaciones.numChannels,samplesPerBlock );
   
    gainDeEntrada.prepare(especificaciones);
    gainDeSalida.prepare(especificaciones);

    gainDeEntrada.setRampDurationSeconds(0.05);//50 ms
    gainDeSalida.setRampDurationSeconds(0.05);

    for (auto& buffer : filterBuffers)
    {
        buffer.setSize(especificaciones.numChannels, samplesPerBlock);
    }

    leftChannelFifo.prepare(samplesPerBlock);
    rightChannelFifo.prepare(samplesPerBlock);
    
    oscilador.initialise([](float x) { return std::sin(x); });
    oscilador.prepare(especificaciones);
    oscilador.setFrequency(getSampleRate() / ((2 << FFTOrder::order2048) - 1)*50);

    gain.prepare(especificaciones);
    gain.setGainDecibels(-12.f);

}

void BandasDeSonidoCompuestoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BandasDeSonidoCompuestoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BandasDeSonidoCompuestoAudioProcessor::actualizaEstado()
{
    for (auto& compresor : compresores)
    {
        compresor.actualizaOpcionesCompresor();
    }

    auto cutoffMedioBajo = bajoMedio->get();
    BP1.setCutoffFrequency(cutoffMedioBajo);
    AP1.setCutoffFrequency(cutoffMedioBajo);

    auto cutoffMedioAlto = altoMedio->get();
    BP2.setCutoffFrequency(cutoffMedioAlto);
    AP2.setCutoffFrequency(cutoffMedioAlto);
    FULLP2.setCutoffFrequency(cutoffMedioAlto);

    gainDeEntrada.setGainDecibels(parametroGainDeEntrada->get());
    gainDeSalida.setGainDecibels(parametroGainDeSalida->get());
    
}

void BandasDeSonidoCompuestoAudioProcessor::troceaBandas(const juce::AudioBuffer<float>& bufferDeEntrada)
{
    for (auto& fb : filterBuffers)
    {
        fb = bufferDeEntrada;
    }



    auto fb0Bloque = juce::dsp::AudioBlock<float>(filterBuffers[0]);
    auto fb1Bloque = juce::dsp::AudioBlock<float>(filterBuffers[1]);
    auto fb2Bloque = juce::dsp::AudioBlock<float>(filterBuffers[2]);

    auto fb0Ctx = juce::dsp::ProcessContextReplacing<float>(fb0Bloque);
    auto fb1Ctx = juce::dsp::ProcessContextReplacing<float>(fb1Bloque);
    auto fb2Ctx = juce::dsp::ProcessContextReplacing<float>(fb2Bloque);

    BP1.process(fb0Ctx);
    FULLP2.process(fb0Ctx);

    AP1.process(fb1Ctx);
    filterBuffers[2] = filterBuffers[1];
    BP2.process(fb1Ctx);

    AP2.process(fb2Ctx);

}

void BandasDeSonidoCompuestoAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //******************************************************************************************************************************

    //*******************************************************************************************************************************
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
   
    //LMP

    actualizaEstado();

    if ( false )
    {
        buffer.clear();
        auto bloque = juce::dsp::AudioBlock<float>(buffer);
        auto contexto = juce::dsp::ProcessContextReplacing<float>(bloque);
        oscilador.process(contexto);
        gain.setGainDecibels(JUCE_LIVE_CONSTANT(-12));
       gain.process(contexto);
    }

    
    leftChannelFifo.update(buffer);
    rightChannelFifo.update(buffer);


    aplicaGain(buffer, gainDeEntrada);

    troceaBandas(buffer);

    for (size_t i = 0; i < filterBuffers.size(); ++i)
    {
        compresores[i].procesa(filterBuffers[i]);
    }
    
    auto numeroDeSamples = buffer.getNumSamples();
    auto numeroDeCanales = buffer.getNumChannels();
  

    buffer.clear();
    
    auto añadeFiltroDeBanda = [c = numeroDeCanales, s = numeroDeSamples](auto& inputBuffer, const auto& fuente)
    {
        for (auto i = 0; i < c; ++i)
        {
            inputBuffer.addFrom(i, 0, fuente, i, 0, s);
        }
    };

    auto bandaSolo = false;
    for (auto& compresor : compresores)
    {
        if (compresor.solo->get())
        {
            bandaSolo = true;
            break;
        }
    }

    if (bandaSolo)
    {
        for (size_t i = 0; i < compresores.size(); ++i)
        {
            auto& compresor = compresores[i];
            if (compresor.solo->get())
            {
                añadeFiltroDeBanda(buffer, filterBuffers[i]);
            }
        }
    }
    else
    {
        for (size_t i = 0; i < compresores.size(); ++i)
        {
            auto& compresor = compresores[i];
            if (!compresor.mute->get())
            {
                añadeFiltroDeBanda(buffer, filterBuffers[i]);
            }
        }
    }

    aplicaGain(buffer, gainDeSalida);

   /* if (compresor.bypass->get())
    {
        for (auto canal = 0; canal < numeroDeCanales; ++canal)
        {
            juce::FloatVectorOperations::multiply(invFULLBuffer.getWritePointer(canal),
                -1.f, numeroDeSamples);
        }
        añadeFiltroDeBanda(buffer, invFULLBuffer);
    }*/

    //compresor.actualizaOpcionesCompresor();
    //compresor.procesa(buffer);
   /* compresor.setAttack(attack->get());
    compresor.setRelease(libera->get());
    compresor.setThreshold(limite->get());
    compresor.setRatio(ratio->getCurrentChoiceName().getFloatValue());

    auto bloque = juce::dsp::AudioBlock<float>(buffer);
    auto contexto = juce::dsp::ProcessContextReplacing<float>(bloque);

    contexto.isBypassed = bypass->get();

    compresor.process(contexto);*/

    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
   // for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channelData = buffer.getWritePointer (channel);
    //
        // ..do something to the data...
    //}
}

//==============================================================================
bool BandasDeSonidoCompuestoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BandasDeSonidoCompuestoAudioProcessor::createEditor()
{
    return new BandasDeSonidoCompuestoAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void BandasDeSonidoCompuestoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    //LMP
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void BandasDeSonidoCompuestoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    //LMP
    auto arbol = juce::ValueTree::readFromData(data, sizeInBytes);

    if (arbol.isValid())
    {
        apvts.replaceState(arbol);
    }
}
//*****************************************************************************************************************

//*****************************************************************************************************************
//LMP
juce::AudioProcessorValueTreeState::ParameterLayout BandasDeSonidoCompuestoAudioProcessor::creaLayoutParam()
{
    APVTS::ParameterLayout layout;

    using namespace juce;
    using namespace Parametros;
    const auto& parametros = GetParametros();

    auto rangoDeGain = NormalisableRange<float>(-24.f,24.f,0.5f,1.f);

    layout.add(std::make_unique<AudioParameterFloat>(
        parametros.at(Nombres::Gain_Entrada),
        parametros.at(Nombres::Gain_Entrada),
        rangoDeGain,
        0));

    layout.add(std::make_unique<AudioParameterFloat>(
        parametros.at(Nombres::Gain_Salida),
        parametros.at(Nombres::Gain_Salida),
        rangoDeGain,
        0));
  
    auto ragoDeLimite = NormalisableRange<float>(LIMITE_MINIMO, DECIBELIOS_MAXIMO, 1, 1);

    layout.add(std::make_unique<AudioParameterFloat>(
                                                    parametros.at(Nombres::Limite_Banda_Bajo),
                                                    parametros.at(Nombres::Limite_Banda_Bajo),
        ragoDeLimite,
                                                    0));

    layout.add(std::make_unique<AudioParameterFloat>(
        parametros.at(Nombres::Limite_Banda_Medio),
        parametros.at(Nombres::Limite_Banda_Medio),
        ragoDeLimite,
        0));

    layout.add(std::make_unique<AudioParameterFloat>(
                                                        parametros.at(Nombres::Limite_Banda_Alto),
                                                        parametros.at(Nombres::Limite_Banda_Alto),
                                                        ragoDeLimite,
                                                        0));



    auto liberaRangoAttack = NormalisableRange<float>(5, 500, 1, 1);

    layout.add(std::make_unique<AudioParameterFloat>(
        parametros.at(Nombres::Attack_Banda_Bajo),
        parametros.at(Nombres::Attack_Banda_Bajo),
        liberaRangoAttack,
        50));

    layout.add(std::make_unique<AudioParameterFloat>(
        parametros.at(Nombres::Attack_Banda_Medio),
        parametros.at(Nombres::Attack_Banda_Medio),
        liberaRangoAttack,
        50));

    layout.add(std::make_unique<AudioParameterFloat>(
        parametros.at(Nombres::Attack_Banda_Alto),
        parametros.at(Nombres::Attack_Banda_Alto),
        liberaRangoAttack,
        50));

    layout.add(std::make_unique<AudioParameterFloat>(
        parametros.at(Nombres::Libera_Banda_Bajo),
        parametros.at(Nombres::Libera_Banda_Bajo),
        liberaRangoAttack,
        250));

    layout.add(std::make_unique<AudioParameterFloat>(
        parametros.at(Nombres::Libera_Banda_Medio),
        parametros.at(Nombres::Libera_Banda_Medio),
        liberaRangoAttack,
        250));

    layout.add(std::make_unique<AudioParameterFloat>(
        parametros.at(Nombres::Libera_Banda_Alto),
        parametros.at(Nombres::Libera_Banda_Alto),
        liberaRangoAttack,
        250));

    auto selecciones = std::vector<double>{ 1,1.5,2,3,4,5,6,7,8,10,15,20,50,100 };
    juce::StringArray sa;
    for (auto seleccion : selecciones)
    {
        sa.add(juce::String(seleccion, 1));
    }

    layout.add(std::make_unique<AudioParameterChoice>(
        parametros.at(Nombres::Ratio_Banda_Bajo),
        parametros.at(Nombres::Ratio_Banda_Bajo),
        sa,
        3));
  
    layout.add(std::make_unique<AudioParameterChoice>(
        parametros.at(Nombres::Ratio_Banda_Medio),
        parametros.at(Nombres::Ratio_Banda_Medio),
        sa,
        3));

    layout.add(std::make_unique<AudioParameterChoice>(
        parametros.at(Nombres::Ratio_Banda_Alto),
        parametros.at(Nombres::Ratio_Banda_Alto),
        sa,
        3));


    layout.add(std::make_unique<AudioParameterBool>(
        parametros.at(Nombres::Bypass_Banda_Bajo),
        parametros.at(Nombres::Bypass_Banda_Bajo),
        false));

    layout.add(std::make_unique<AudioParameterBool>(
        parametros.at(Nombres::Bypass_Banda_Medio),
        parametros.at(Nombres::Bypass_Banda_Medio),
        false));

    layout.add(std::make_unique<AudioParameterBool>(
        parametros.at(Nombres::Bypass_Banda_Alto),
        parametros.at(Nombres::Bypass_Banda_Alto),
        false));

    layout.add(std::make_unique<AudioParameterBool>(
        parametros.at(Nombres::Solo_Banda_Bajo),
        parametros.at(Nombres::Solo_Banda_Bajo),
        false));

    layout.add(std::make_unique<AudioParameterBool>(
        parametros.at(Nombres::Solo_Banda_Medio),
        parametros.at(Nombres::Solo_Banda_Medio),
        false));

    layout.add(std::make_unique<AudioParameterBool>(
        parametros.at(Nombres::Solo_Banda_Alto),
        parametros.at(Nombres::Solo_Banda_Alto),
        false));

    layout.add(std::make_unique<AudioParameterBool>(
        parametros.at(Nombres::Mute_Banda_Bajo),
        parametros.at(Nombres::Mute_Banda_Bajo),
        false));

    layout.add(std::make_unique<AudioParameterBool>(
        parametros.at(Nombres::Mute_Banda_Medio),
        parametros.at(Nombres::Mute_Banda_Medio),
        false));

    layout.add(std::make_unique<AudioParameterBool>(
        parametros.at(Nombres::Mute_Banda_Alto),
        parametros.at(Nombres::Mute_Banda_Alto),
        false));

    layout.add(std::make_unique<AudioParameterFloat>(parametros.at(Nombres::Medio_Bajo_Freq_Tras),
                                                    parametros.at(Nombres::Medio_Bajo_Freq_Tras),
                                                    NormalisableRange<float>(FRECUENCIA_MINIMA,999,1,1),
                                                    400));
    
    layout.add(std::make_unique<AudioParameterFloat>(parametros.at(Nombres::Medio_Alto_Freq_Tras),
        parametros.at(Nombres::Medio_Alto_Freq_Tras),
        NormalisableRange<float>(1000, FRECUENCIA_MAXIMA, 1, 1),
        2000));
    //************************************************************************************************************************

    //*****************************************************************************************************************************
    
    return layout;
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BandasDeSonidoCompuestoAudioProcessor();
}
