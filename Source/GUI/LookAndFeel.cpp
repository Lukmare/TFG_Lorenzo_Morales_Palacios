/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 5 May 2023 2:46:08pm
    Author:  loren

  ==============================================================================
*/

#include "LookAndFeel.h"
#include "SliderRotatorioConEtiquetas.h"
#include "BotonesPersonalizados.h"
void LookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x,
    int y,
    int width,
    int height,
    float sliderPosProportional,
    float anguloDeRotacionDeComienzo,
    float anguloDeRotacionDeFin,
    juce::Slider& slider)
{
    using namespace juce;

    auto bounds = Rectangle<float>(x, y, width, height);

    auto activo = slider.isEnabled();

    g.setColour(activo ? Colour(97u, 114u, 158u) : Colours::darkgrey);
    g.fillEllipse(bounds);

    g.setColour(activo ? Colour(198u, 204u, 220u) : Colours::grey);
    g.drawEllipse(bounds, 1.f);

    if (auto* rswl = dynamic_cast<SliderRotatorioConEtiquetas*>(&slider))
    {
        auto centro = bounds.getCentre();
        Path p;

        Rectangle<float> r;
        r.setLeft(centro.getX() - 2);
        r.setRight(centro.getX() + 2);
        r.setTop(bounds.getY());
        r.setBottom(centro.getY() - rswl->getTextHeight() * 1.5);

        p.addRoundedRectangle(r, 2.f);

        jassert(anguloDeRotacionDeComienzo < anguloDeRotacionDeFin);

        auto sliderAngRad = jmap(sliderPosProportional, 0.f, 1.f, anguloDeRotacionDeComienzo, anguloDeRotacionDeFin);

        p.applyTransform(AffineTransform().rotated(sliderAngRad, centro.getX(), centro.getY()));

        g.fillPath(p);

        g.setFont(rswl->getTextHeight());
        auto text = rswl->getDisplayString();
        auto strWidth = g.getCurrentFont().getStringWidth(text);

        r.setSize(strWidth + 4, rswl->getTextHeight() + 2);
        r.setCentre(bounds.getCentre());

        g.setColour(activo ? Colours::black : Colours::darkgrey);
        g.fillRect(r);

        g.setColour(activo ? Colours::white : Colours::lightgrey);
        g.drawFittedText(text, r.toNearestInt(), juce::Justification::centred, 1);
    }
}

void LookAndFeel::drawToggleButton(juce::Graphics& g,
    juce::ToggleButton& toggleButton,
    bool dibujaBotonIluminado,
    bool dibujaBotonApagado)
{
    using namespace juce;

    if (auto* pb = dynamic_cast<BotonDeEncendido*>(&toggleButton))
    {
        Path botonDeEncendido;

        auto bounds = toggleButton.getLocalBounds();

        auto size = jmin(bounds.getWidth(), bounds.getHeight()) - 6;
        auto r = bounds.withSizeKeepingCentre(size, size).toFloat();

        float ang = 30.f; //30.f;

        size -= 6;

        botonDeEncendido.addCentredArc(r.getCentreX(),
            r.getCentreY(),
            size * 0.5,
            size * 0.5,
            0.f,
            degreesToRadians(ang),
            degreesToRadians(360.f - ang),
            true);

        botonDeEncendido.startNewSubPath(r.getCentreX(), r.getY());
        botonDeEncendido.lineTo(r.getCentre());

        PathStrokeType pst(2.f, PathStrokeType::JointStyle::curved);

        auto color = toggleButton.getToggleState() ? Colours::dimgrey : Colour(0u, 172u, 1u);

        g.setColour(color);
        g.strokePath(botonDeEncendido, pst);
        g.drawEllipse(r, 2);
    }
    else if (auto* analyzerButton = dynamic_cast<BotonDelAnalizador*>(&toggleButton))
    {
        auto color = !toggleButton.getToggleState() ? Colours::dimgrey : Colour(0u, 172u, 1u);

        g.setColour(color);

        auto bounds = toggleButton.getLocalBounds();
        g.drawRect(bounds);

        g.strokePath(analyzerButton->pathAleatorio, PathStrokeType(1.f));
    }
    else
    {
        auto bounds = toggleButton.getLocalBounds().reduced(2);

        auto botonActivo = toggleButton.getToggleState();

        const int tamañoDeEsquina = 4;

        g.setColour(botonActivo ? toggleButton.findColour(TextButton::ColourIds::buttonOnColourId) : toggleButton.findColour(TextButton::ColourIds::buttonColourId));
        g.fillRoundedRectangle(bounds.toFloat(), tamañoDeEsquina);
        g.setColour(botonActivo ? juce::Colours::black : juce::Colours::white);
        g.drawRoundedRectangle(bounds.toFloat(), tamañoDeEsquina, 1);
        g.drawFittedText(toggleButton.getName(), bounds, juce::Justification::centred, 1);
    }
}