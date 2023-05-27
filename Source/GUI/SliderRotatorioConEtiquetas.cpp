/*
  ==============================================================================

    SliderRotatorioConEtiquetas.cpp
    Created: 5 May 2023 2:54:08pm
    Author:  loren

  ==============================================================================
*/

#include "SliderRotatorioConEtiquetas.h"
#include "Funcionalidades.h"

void SliderRotatorioConEtiquetas::paint(juce::Graphics& g)
{
    using namespace juce;

    auto anguloDeInicio = degreesToRadians(180.f + 45.f);
    auto anguloDeFin = degreesToRadians(180.f - 45.f) + MathConstants<float>::twoPi;

    auto rango = getRange();

    auto sliderBounds = getSliderBounds();

    auto bounds = getLocalBounds();

    g.setColour(Colours::aliceblue);
    g.drawFittedText(getName(), bounds.removeFromTop(getTextHeight() + 2),
        Justification::centredBottom, 1);

    //    g.setColour(Colours::red);
    //    g.drawRect(getLocalBounds());
    //    g.setColour(Colours::yellow);
    //    g.drawRect(sliderBounds);

    getLookAndFeel().drawRotarySlider(g,
        sliderBounds.getX(),
        sliderBounds.getY(),
        sliderBounds.getWidth(),
        sliderBounds.getHeight(),
        jmap(getValue(), rango.getStart(), rango.getEnd(), 0.0, 1.0),
        anguloDeInicio,
        anguloDeFin,
        *this);

    auto centro = sliderBounds.toFloat().getCentre();
    auto radio = sliderBounds.getWidth() * 0.5f;

    g.setColour(Colour(0u, 172u, 1u));
    g.setFont(getTextHeight());

    auto numeroChoices = etiquetas.size();
    for (int i = 0; i < numeroChoices; ++i)
    {
        auto pos = etiquetas[i].pos;
        jassert(0.f <= pos);
        jassert(pos <= 1.f);

        auto ang = jmap(pos, 0.f, 1.f, anguloDeInicio, anguloDeFin);

        auto c = centro.getPointOnCircumference(radio + getTextHeight() * 0.5f + 1, ang);

        Rectangle<float> r;
        auto str = etiquetas[i].etiqueta;
        r.setSize(g.getCurrentFont().getStringWidth(str), getTextHeight());
        r.setCentre(c);
        r.setY(r.getY() + getTextHeight());

        g.drawFittedText(str, r.toNearestInt(), juce::Justification::centred, 1);
    }

}

juce::Rectangle<int> SliderRotatorioConEtiquetas::getSliderBounds() const
{
    auto bounds = getLocalBounds();

    bounds.removeFromTop(getTextHeight() * 1.5);

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());

    size -= getTextHeight() * 1.5;
    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), 0);
    //r.setY(2);
    r.setY(bounds.getY());
    return r;

}

juce::String SliderRotatorioConEtiquetas::getDisplayString() const
{
    if (auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(parametro))
        return choiceParam->getCurrentChoiceName();

    juce::String str;
    bool añadeK = false;

    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(parametro))
    {
        float val = getValue();

        //     if (val > 999.f)
        //   {
        //         val /= 1000.f; //1001 / 1000 = 1.001
        //         añadeK = true;
        //     }
        añadeK = truncadoKV(val);
        str = juce::String(val, (añadeK ? 2 : 0));
    }
    else
    {
        jassertfalse; //this shouldn't happen!
    }

    if (sufijo.isNotEmpty())
    {
        str << " ";
        if (añadeK)
            str << "k";

        str << sufijo;
    }

    return str;
}


void SliderRotatorioConEtiquetas::cambiaParametro(juce::RangedAudioParameter* p)
{
    parametro = p;
    repaint();
}

juce::String SliderRatio::getDisplayString() const
{
    auto choiceParam = dynamic_cast<juce::AudioParameterChoice*>(parametro);
    jassert(choiceParam != nullptr);

    auto currentChoice = choiceParam->getCurrentChoiceName();
    if (currentChoice.contains(".0"))
        currentChoice = currentChoice.substring(0, currentChoice.indexOf("."));

    currentChoice << ":1";

    return currentChoice;


}

