/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/BandaCompresor.h"
#include "DSP/CanalSimpleFifo.h"
#include <juce_gui_extra/juce_gui_extra.h>

//==============================================================================
/**
* 
*/

//*******************************************************************************************************************************

//***********************************************************************************************************************
class BandasDeSonidoCompuestoAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    BandasDeSonidoCompuestoAudioProcessor();
    ~BandasDeSonidoCompuestoAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    using APVTS = juce::AudioProcessorValueTreeState; //LMP
    static APVTS::ParameterLayout creaLayoutParam();//createParameterLayout()

    APVTS apvts {*this, nullptr, "Parametros",
    creaLayoutParam()};
     

    using BlockType = juce::AudioBuffer<float>;
    SingleChannelSampleFifo<BlockType> leftChannelFifo{ Channel::Left };
    SingleChannelSampleFifo<BlockType> rightChannelFifo{ Channel::Right };



    std::array<BandaCompresor,3>  compresores;
    BandaCompresor& bandaCompresorBajo = compresores[0];
    BandaCompresor& bandaCompresorMedio = compresores[1];
    BandaCompresor& bandaCompresorAlto = compresores[2];

private:
    //*****************************************************************************************************************************

    //*****************************************************************************************************************************

    using Filtro = juce::dsp::LinkwitzRileyFilter<float>;
   //     Banda1 Banda2
    Filtro BP1, FULLP2,
           AP1, BP2,
                AP2
        ;

   // Filtro invFULLP1, invFULLP2;
   // juce::AudioBuffer<float> invFULLBuffer;

    juce::AudioParameterFloat* bajoMedio{ nullptr };
    juce::AudioParameterFloat* altoMedio{ nullptr };

    std::array<juce::AudioBuffer<float>, 3> filterBuffers;

    juce::dsp::Gain<float> gainDeEntrada, gainDeSalida;
    juce::AudioParameterFloat* parametroGainDeEntrada{ nullptr };
    juce::AudioParameterFloat* parametroGainDeSalida{ nullptr };
    template<typename T, typename U>
    void aplicaGain(T& buffer, U& gain)
    {
        auto bloque = juce::dsp::AudioBlock<float>(buffer);
        auto contexto = juce::dsp::ProcessContextReplacing<float>(bloque);
        gain.process(contexto);
    }

    void actualizaEstado();

    void troceaBandas(const juce::AudioBuffer<float>& bufferDeEntrada);

    juce::dsp::Oscillator<float> oscilador;
    juce::dsp::Gain<float> gain;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandasDeSonidoCompuestoAudioProcessor)
};
