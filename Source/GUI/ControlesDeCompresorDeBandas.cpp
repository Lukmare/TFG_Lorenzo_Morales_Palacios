/*
  ==============================================================================

    ControlesDeCompresorDeBandas.cpp
    Created: 5 May 2023 3:19:08pm
    Author:  loren

  ==============================================================================
*/

#include "ControlesDeCompresorDeBandas.h"
#include "Funcionalidades.h"
#include "../DSP/Parametros.h"


ControlesDeCompresorDeBandas::ControlesDeCompresorDeBandas(juce::AudioProcessorValueTreeState& apv) :
    apvts(apv), sliderDeAttack(nullptr, "ms", "ATTACK"), sliderDeLibera(nullptr, "ms", "LIBERA"), sliderDeLimite(nullptr, "dB", "LIMITE"), sliderDeRatio(nullptr, "")
{
    using namespace Parametros;
    const auto& parametros = GetParametros();
    /*
    auto getParametroHelper = [&parametros, &apvts = this->apvts](const auto& nombre)-> auto&
    {
        return getParametro(apvts, parametros, nombre);
    };

    sliderDeAttack.cambiaParametro(&getParametroHelper(Nombres::Attack_Banda_Medio));
    sliderDeLimite.cambiaParametro(&getParametroHelper(Nombres::Limite_Banda_Medio));
    sliderDeRatio.cambiaParametro(&getParametroHelper(Nombres::Ratio_Banda_Medio));
    sliderDeLibera.cambiaParametro(&getParametroHelper(Nombres::Libera_Banda_Medio));

    añadeEtiquetas(sliderDeAttack.etiquetas, getParametroHelper(Nombres::Attack_Banda_Medio), "ms");
    añadeEtiquetas(sliderDeLibera.etiquetas, getParametroHelper(Nombres::Libera_Banda_Medio), "ms");
    añadeEtiquetas(sliderDeLimite.etiquetas, getParametroHelper(Nombres::Limite_Banda_Medio), "dB");

    sliderDeRatio.etiquetas.clear();
    sliderDeRatio.etiquetas.add({ 0.f,"1:1" });
    auto parametroRatio = dynamic_cast<juce::AudioParameterChoice*>(&getParametroHelper(Nombres::Ratio_Banda_Medio));

    sliderDeRatio.etiquetas.add({ 1.0f,parametroRatio->choices.getReference(parametroRatio->choices.size() - 1) });
    */
    auto creaAttachmentHelper = [&parametros, &apvts = this->apvts](auto& attachment, const auto& nombre, auto& slider)
    {
        creaAttachment(attachment, apvts, parametros, nombre, slider);
    };

    creaAttachmentHelper(sliderDeAttackAttachment,
        Nombres::Attack_Banda_Medio,
        sliderDeAttack);

    creaAttachmentHelper(sliderDeLiberaAttachment,
        Nombres::Libera_Banda_Medio,
        sliderDeLibera);

    creaAttachmentHelper(sliderDeLimiteAttachment,
        Nombres::Limite_Banda_Medio,
        sliderDeLimite);

    creaAttachmentHelper(sliderDeRatioAttachment,
        Nombres::Ratio_Banda_Medio,
        sliderDeRatio);

    addAndMakeVisible(sliderDeAttack);
    addAndMakeVisible(sliderDeLibera);
    addAndMakeVisible(sliderDeLimite);
    addAndMakeVisible(sliderDeRatio);


    botonBypass.addListener(this);
    botonSolo.addListener(this);
    botonMute.addListener(this);


    botonBypass.setName("B");
    botonBypass.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkcyan);
    botonBypass.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);

    botonSolo.setName("S");
    botonSolo.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::rebeccapurple);
    botonSolo.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);

    botonMute.setName("M");
    botonMute.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::mediumvioletred);
    botonMute.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);


    addAndMakeVisible(botonBypass);
    addAndMakeVisible(botonSolo);
    addAndMakeVisible(botonMute);

    creaAttachmentHelper(botonBypassAttachment, Nombres::Bypass_Banda_Medio, botonBypass);
    creaAttachmentHelper(botonSoloAttachment, Nombres::Solo_Banda_Medio, botonSolo);
    creaAttachmentHelper(botonMuteAttachment, Nombres::Mute_Banda_Medio, botonMute);

    bandaBajo.setName("Bajo");
    bandaBajo.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::grey);
    bandaBajo.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);

    bandaAlto.setName("Alto");
    bandaAlto.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::grey);
    bandaAlto.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);

    bandaMedio.setName("Medio");
    bandaMedio.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::grey);
    bandaMedio.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);

    bandaBajo.setRadioGroupId(1);
    bandaAlto.setRadioGroupId(1);
    bandaMedio.setRadioGroupId(1);

    auto cambioEnBoton = [punteroSeguro = this->punteroSeguro]()
    {
        if (auto* c = punteroSeguro.getComponent())
        {
            c->actualizaAttachments();
        }
    };

    bandaBajo.onClick = cambioEnBoton;
    bandaAlto.onClick = cambioEnBoton;
    bandaMedio.onClick = cambioEnBoton;

    bandaBajo.setToggleState(true, juce::NotificationType::dontSendNotification);
    actualizaAttachments();

    actualizaAttachments();
    actualizaHabilitadoresDeSlider();
    actualizaEstadoSeleccionado();

    addAndMakeVisible(bandaBajo);
    addAndMakeVisible(bandaMedio);
    addAndMakeVisible(bandaAlto);

}


ControlesDeCompresorDeBandas::~ControlesDeCompresorDeBandas()
{
    botonBypass.removeListener(this);
    botonSolo.removeListener(this);
    botonMute.removeListener(this);



}

void ControlesDeCompresorDeBandas::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    using namespace juce;

    auto creaCajaBotonControlDeBanda = [](std::vector<Component*> componentes)
    {
        FlexBox fb;
        fb.flexDirection = FlexBox::Direction::column;
        fb.flexWrap = FlexBox::Wrap::noWrap;
        auto espaciado = FlexItem().withHeight(2);

        for (auto* componente : componentes)
        {
            fb.items.add(espaciado);
            fb.items.add(FlexItem(*componente).withFlex(1.f));
        }

        fb.items.add(espaciado);

        return fb;
    };


    auto botonDeControlDeBanda = creaCajaBotonControlDeBanda({ &botonBypass,&botonSolo,&botonMute });
    auto botonDeSeleccionDeBanda = creaCajaBotonControlDeBanda({ &bandaBajo,&bandaMedio,&bandaAlto });


    FlexBox flexB;
    flexB.flexDirection = FlexBox::Direction::row;
    flexB.flexWrap = FlexBox::Wrap::noWrap;

    auto espaciado = FlexItem().withWidth(4);
    auto finCap = FlexItem().withWidth(6);

    flexB.items.add(espaciado);
    flexB.items.add(FlexItem(botonDeSeleccionDeBanda).withWidth(55));
    flexB.items.add(espaciado);
    flexB.items.add(FlexItem(sliderDeAttack).withFlex(1.f));
    flexB.items.add(espaciado);
    flexB.items.add(FlexItem(sliderDeLibera).withFlex(1.f));
    flexB.items.add(espaciado);
    flexB.items.add(FlexItem(sliderDeLimite).withFlex(1.f));
    flexB.items.add(espaciado);
    flexB.items.add(FlexItem(sliderDeRatio).withFlex(1.f));
    flexB.items.add(espaciado);
    flexB.items.add(FlexItem(botonDeControlDeBanda).withWidth(30));

    flexB.performLayout(bounds);

}

void ControlesDeCompresorDeBandas::paint(juce::Graphics& x)
{
    auto bounds = getLocalBounds();
    dibujaFondo(x, bounds);

}

void ControlesDeCompresorDeBandas::buttonClicked(juce::Button* boton)
{
    actualizaHabilitadoresDeSlider();
    actualizaSMBEstados(*boton);
    actualizaColoresDeBandaActiva(*boton);
}
//LMP
void ControlesDeCompresorDeBandas::muestraTodasLasBandas(bool bypassActivo)
{
    std::vector<Component*> bandas{ &bandaBajo
,&bandaMedio,&bandaAlto };
    for(auto* banda: bandas)
    {
        banda->setColour(juce::TextButton::ColourIds::buttonOnColourId,
            bypassActivo ? botonBypass.findColour(juce::TextButton::ColourIds::buttonOnColourId) : juce::Colours::lightgrey);

        banda->setColour(juce::TextButton::ColourIds::buttonColourId,
            bypassActivo ? botonBypass.findColour(juce::TextButton::ColourIds::buttonOnColourId) : juce::Colours::black
        );

        banda->repaint();



    }
}
void ControlesDeCompresorDeBandas::actualizaColoresDeBandaActiva(juce::Button &botonActivado)
{
    jassert(bandaActiva != nullptr);
    DBG("banda activa: " << bandaActiva->getName());

    if(botonActivado.getToggleState() == false)
    {
        resetColoresDeBandaActiva();
    }
    else
    {
        refrescaColoresDeBandaActiva(*bandaActiva, botonActivado);
    }
}

void ControlesDeCompresorDeBandas::refrescaColoresDeBandaActiva(juce::Button &banda, juce::Button &color) {

    banda.setColour(juce::TextButton::ColourIds::buttonOnColourId,
        color.findColour(juce::TextButton::ColourIds::buttonOnColourId));

    banda.setColour(juce::TextButton::ColourIds::buttonColourId,
        color.findColour(juce::TextButton::ColourIds::buttonOnColourId));
    banda.repaint();
}
void ControlesDeCompresorDeBandas::resetColoresDeBandaActiva()
{
    bandaActiva->setColour(juce::TextButton::ColourIds::buttonOnColourId,
        juce::Colours::grey);

    bandaActiva->setColour(juce::TextButton::ColourIds::buttonColourId,
        juce::Colours::transparentBlack);

    bandaActiva->repaint();
}

void ControlesDeCompresorDeBandas::actualizaEstadoSeleccionado()
{
    using namespace Parametros;

    std::vector<std::array<Nombres, 3>> parametrosComprueba
    {
        {Nombres::Solo_Banda_Bajo, Nombres::Mute_Banda_Bajo, Nombres::Bypass_Banda_Bajo},
                {Nombres::Solo_Banda_Medio, Nombres::Mute_Banda_Medio, Nombres::Bypass_Banda_Medio},
        {Nombres::Solo_Banda_Alto, Nombres::Mute_Banda_Alto, Nombres::Bypass_Banda_Alto}
    };

    const auto& parametros = GetParametros();
    auto parametroHelper = [&parametros, this](const auto& nombre)
    {
        return dynamic_cast<juce::AudioParameterBool*>(&getParametro(apvts, parametros, nombre));
    };

    for (size_t i = 0; i < parametrosComprueba.size(); ++i)
    {
        auto& lista = parametrosComprueba[i];

        auto* botonBanda = (i == 0) ? &bandaBajo : (i == 1) ? &bandaMedio : &bandaAlto;

        if (auto* solo = parametroHelper(lista[0]);
            solo->get())
        {
            refrescaColoresDeBandaActiva(*botonBanda, botonSolo);
        }
        else if (auto* mute = parametroHelper(lista[0]);
            mute->get())
        {
            refrescaColoresDeBandaActiva(*botonBanda, botonMute);
        }
        else if (auto* bypass = parametroHelper(lista[0]);
            bypass->get())
        {
            refrescaColoresDeBandaActiva(*botonBanda, botonBypass);

        }

    }

}

void ControlesDeCompresorDeBandas::actualizaHabilitadoresDeSlider()
{
    auto desactivado = botonMute.getToggleState() || botonBypass.getToggleState();

    sliderDeAttack.setEnabled(!desactivado);
    sliderDeLibera.setEnabled(!desactivado);
    sliderDeLimite.setEnabled(!desactivado);
    sliderDeRatio.setEnabled(!desactivado);

}

void ControlesDeCompresorDeBandas::actualizaSMBEstados(juce::Button& botoActivado)
{
    if (&botoActivado == &botonSolo && botonSolo.getToggleState())
    {
        botonBypass.setToggleState(false, juce::NotificationType::sendNotification);
        botonMute.setToggleState(false, juce::NotificationType::sendNotification);

    }
    if (&botoActivado == &botonMute && botonMute.getToggleState())
    {
        botonBypass.setToggleState(false, juce::NotificationType::sendNotification);
        botonSolo.setToggleState(false, juce::NotificationType::sendNotification);

    }
    if (&botoActivado == &botonBypass && botonBypass.getToggleState())
    {
        botonSolo.setToggleState(false, juce::NotificationType::sendNotification);
        botonMute.setToggleState(false, juce::NotificationType::sendNotification);

    }
}

void ControlesDeCompresorDeBandas::actualizaAttachments()
{
    enum TipoBanda
    {
        Bajo, Medio, Alto
    };

    TipoBanda tipoBanda = [this]()
    {
        if (bandaBajo.getToggleState())
        {

            return TipoBanda::Bajo;
        }
        if (bandaMedio.getToggleState())
        {
            return TipoBanda::Medio;
        }

        return TipoBanda::Alto;
    }();
    using namespace Parametros;
    std::vector<Nombres> nombres;

    switch (tipoBanda)
    {
    case Bajo:
    {
        nombres = std::vector<Nombres>
        {
            Nombres::Attack_Banda_Bajo
            ,Nombres::Libera_Banda_Bajo,
            Nombres::Limite_Banda_Bajo

            ,Nombres::Ratio_Banda_Bajo
            ,Nombres::Mute_Banda_Bajo
            ,Nombres::Solo_Banda_Bajo,
            Nombres::Bypass_Banda_Bajo
        };

        bandaActiva = &bandaBajo;
        break;
    }
    case Medio:
    {
        nombres = std::vector<Nombres>
        {
            Nombres::Attack_Banda_Medio
            ,Nombres::Libera_Banda_Medio,
            Nombres::Limite_Banda_Medio

            ,Nombres::Ratio_Banda_Medio
            ,Nombres::Mute_Banda_Medio
            ,Nombres::Solo_Banda_Medio,
            Nombres::Bypass_Banda_Medio
        };

        bandaActiva = &bandaMedio;

        break;
    }
    case Alto:
    {
        nombres = std::vector<Nombres>
        {
            Nombres::Attack_Banda_Alto
            ,Nombres::Libera_Banda_Alto,
            Nombres::Limite_Banda_Alto

            ,Nombres::Ratio_Banda_Alto
            ,Nombres::Mute_Banda_Alto
            ,Nombres::Solo_Banda_Alto,
            Nombres::Bypass_Banda_Alto
        };

        bandaActiva = &bandaAlto;

        break;
    }
    }

    enum Posicion
    {
        Attack,
        Libera, Limite, Ratio, Mute, Solo, Bypass
    };

    const auto& parametros = GetParametros();

    auto getParametroHelper = [&parametros, &apvts = this->apvts, &nombres](const auto& posicion)-> auto&
    {
        return getParametro(apvts, parametros, nombres.at(posicion));
    };

    sliderDeAttackAttachment.reset();
    sliderDeLiberaAttachment.reset();

    sliderDeLimiteAttachment.reset();

    sliderDeRatioAttachment.reset();

    botonBypassAttachment.reset();

    botonMuteAttachment.reset();

    botonSoloAttachment.reset();

    auto& parametroAttack = getParametroHelper(Posicion::Attack);
    añadeEtiquetas(sliderDeAttack.etiquetas, parametroAttack, "ms");
    sliderDeAttack.cambiaParametro(&parametroAttack);

    auto& parametroLibera = getParametroHelper(Posicion::Libera);
    añadeEtiquetas(sliderDeLibera.etiquetas, parametroLibera, "ms");
    sliderDeLibera.cambiaParametro(&parametroLibera);

    auto& parametroLimite = getParametroHelper(Posicion::Limite);
    añadeEtiquetas(sliderDeLimite.etiquetas, parametroLimite, "dB");
    sliderDeLimite.cambiaParametro(&parametroLimite);


    auto& parametroRatioRap = getParametroHelper(Posicion::Ratio);

    sliderDeRatio.etiquetas.clear();
    sliderDeRatio.etiquetas.add({ 0.f,"1:1" });
    auto parametroRatio = dynamic_cast<juce::AudioParameterChoice*>(&parametroRatioRap);

    sliderDeRatio.etiquetas.add({ 1.0f,parametroRatio->choices.getReference(parametroRatio->choices.size() - 1) });

    sliderDeRatio.cambiaParametro(parametroRatio);

    auto creaAttachmentHelper = [&parametros, &apvts = this->apvts](auto& attachment, const auto& nombre, auto& slider)
    {
        creaAttachment(attachment, apvts, parametros, nombre, slider);
    };

    creaAttachmentHelper(sliderDeAttackAttachment, nombres[Posicion::Attack], sliderDeAttack);
    creaAttachmentHelper(sliderDeLiberaAttachment, nombres[Posicion::Libera], sliderDeLibera);
    creaAttachmentHelper(sliderDeLimiteAttachment, nombres[Posicion::Limite], sliderDeLimite);
    creaAttachmentHelper(sliderDeRatioAttachment, nombres[Posicion::Ratio], sliderDeRatio);
    creaAttachmentHelper(botonBypassAttachment, nombres[Posicion::Bypass], botonBypass);
    creaAttachmentHelper(botonSoloAttachment, nombres[Posicion::Solo], botonSolo);
    creaAttachmentHelper(botonMuteAttachment, nombres[Posicion::Mute], botonMute);


}

