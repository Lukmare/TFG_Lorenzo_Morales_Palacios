/*
  ==============================================================================

    Parametros.h
    Created: 5 May 2023 3:33:03pm
    Author:  loren

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Parametros
{

    enum Nombres {


        Medio_Bajo_Freq_Tras,
        Medio_Alto_Freq_Tras,

        Limite_Banda_Bajo,
        Limite_Banda_Alto,
        Limite_Banda_Medio,

        Attack_Banda_Bajo,
        Attack_Banda_Alto,
        Attack_Banda_Medio,

        Libera_Banda_Bajo,
        Libera_Banda_Alto,
        Libera_Banda_Medio,

        Ratio_Banda_Medio,
        Ratio_Banda_Bajo,
        Ratio_Banda_Alto,

        Bypass_Banda_Bajo,
        Bypass_Banda_Alto,
        Bypass_Banda_Medio,

        Mute_Banda_Bajo,
        Mute_Banda_Alto,
        Mute_Banda_Medio,

        Solo_Banda_Bajo,
        Solo_Banda_Alto,
        Solo_Banda_Medio,

        Gain_Entrada,
        Gain_Salida,

    };

    inline const std::map<Nombres, juce::String>& GetParametros()
    {
        static std::map<Nombres, juce::String> parametros =
        {

        {Medio_Bajo_Freq_Tras,"Frecuencia Banda Bajo Medio"},
        {Medio_Alto_Freq_Tras,"Frecuencia Banda Alto Medio" },

        { Limite_Banda_Bajo, "Limite Banda Bajo"},
        { Limite_Banda_Alto, "Limite Banda Alto"},
        { Limite_Banda_Medio, "Limite Banda Medio"},

        { Attack_Banda_Bajo, "Attack Banda Bajo"},
        { Attack_Banda_Alto, "Attack Banda Alto"},
        { Attack_Banda_Medio, "Attack Banda Medio"},

        { Libera_Banda_Bajo, "Libera Banda Bajo"},
        { Libera_Banda_Alto, "Libera Banda Alto"},
        { Libera_Banda_Medio, "Libera Banda Medio"},

        { Ratio_Banda_Medio, "Ratio Banda Medio"},
        { Ratio_Banda_Bajo, "Ratio Banda Bajo"},
        { Ratio_Banda_Alto, "Ratio Banda Alto"},

        { Bypass_Banda_Bajo, "Bypass Banda Bajo"},
        { Bypass_Banda_Alto, "Bypass Banda Alto"},
        {Bypass_Banda_Medio, "Bypass Banda Medio"},

        { Mute_Banda_Bajo, "Mute Banda Bajo" },
        { Mute_Banda_Alto, "Mute Banda Alto" },
        { Mute_Banda_Medio, "Mute Banda Medio" },

        { Solo_Banda_Bajo, "Solo Banda Bajo" },
        { Solo_Banda_Alto, "Solo Banda Alto" },
        { Solo_Banda_Medio, "Solo Banda Medio" },

        { Gain_Entrada, "Gain de Entrada" },
        { Gain_Salida, "Gain de Salida" }
        };

        return parametros;
    }

}


