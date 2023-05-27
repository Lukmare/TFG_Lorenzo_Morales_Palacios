/*
  ==============================================================================

    ComponenteParaConstruirOnda.cpp
    Created: 5 May 2023 8:45:18pm
    Author:  loren

  ==============================================================================
*/

#include "ComponenteParaConstruirOnda.h"
#include "Funcionalidades.h"
#include "../DSP/Parametros.h"

ComponenteAnalizador::ComponenteAnalizador(BandasDeSonidoCompuestoAudioProcessor& p) :
    audioProcessor(p),
    leftPathProducer(audioProcessor.leftChannelFifo),
    rightPathProducer(audioProcessor.rightChannelFifo)
{
    const auto& params = audioProcessor.getParameters();
    for (auto param : params)
    {
        param->addListener(this);
    }

    using namespace Parametros;
    const auto& nombParametros = GetParametros();

    auto floatHelper = [&apvts = audioProcessor.apvts, &nombParametros](auto& parametro, const auto& nombreParametro) {

        parametro = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(nombParametros.at(nombreParametro)));
        jassert(parametro != nullptr);

    };


    floatHelper(bajoParametroPtr, Nombres::Medio_Bajo_Freq_Tras);
    floatHelper(altoParametroPtr, Nombres::Medio_Alto_Freq_Tras);

    floatHelper(bajoLimiteParametroPtr, Nombres::Limite_Banda_Bajo);
    floatHelper(medioLimiteParametroPtr, Nombres::Limite_Banda_Medio);
    floatHelper(altoLimiteParametroPtr, Nombres::Limite_Banda_Alto);

    startTimerHz(60);
}

ComponenteAnalizador::~ComponenteAnalizador()
{
    const auto& params = audioProcessor.getParameters();
    for (auto param : params)
    {
        param->removeListener(this);
    }
}
//LMP
void ComponenteAnalizador::dibujaAnalisisFFT(juce::Graphics& g, juce::Rectangle<int> bounds)
{

    using namespace juce;
    auto responseArea = getAnalysisArea(bounds);

    Graphics::ScopedSaveState sss(g);
    g.reduceClipRegion(responseArea);

    auto leftChannelFFTPath = leftPathProducer.getPath();
    leftChannelFFTPath.applyTransform(AffineTransform().translation(responseArea.getX(), 0));

    g.setColour(Colour(97u, 18u, 167u)); 
    g.strokePath(leftChannelFFTPath, PathStrokeType(1.f));

    auto rightChannelFFTPath = rightPathProducer.getPath();
    rightChannelFFTPath.applyTransform(AffineTransform().translation(responseArea.getX(), 0)); // responseArea.getY()));

    g.setColour(Colour(215u, 201u, 134u));
    g.strokePath(rightChannelFFTPath, PathStrokeType(1.f));
}

void ComponenteAnalizador::paint(juce::Graphics& g)
{
    using namespace juce;
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::black);

    //LMP
    auto bounds = dibujaFondo(g, getLocalBounds());

    drawBackgroundGrid(g, bounds);

    

    if (shouldShowFFTAnalysis)
    {
        //LMP
        dibujaAnalisisFFT(g, bounds);
       
    }


  /*  Path border;

    border.setUsingNonZeroWinding(false);

    border.addRoundedRectangle(getRenderArea(bounds), 4);
    border.addRectangle(getLocalBounds());

    g.setColour(Colours::black);*/

    dibujaFondos(g, bounds);
    drawTextLabels(g, bounds);

   /* g.setColour(Colours::orange);
    g.drawRoundedRectangle(getRenderArea(bounds).toFloat(), 4.f, 1.f);
    */
    }
void ComponenteAnalizador::dibujaFondos(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    using namespace juce;
    bounds = getAnalysisArea(bounds);

    const auto top = bounds.getY();
    const auto bottom = bounds.getBottom();
    const auto izquierda = bounds.getX();
    const auto derecha = bounds.getRight();

    auto mapaX = [izquierda = bounds.getX(), tamaño = bounds.getWidth()](float frecuencia)
    {
        auto normX = juce::mapFromLog10(frecuencia, FRECUENCIA_MINIMA, FRECUENCIA_MAXIMA);

        return izquierda + tamaño * normX;
    };

    auto bajoX = mapaX(bajoParametroPtr->get());
    g.setColour(Colours::mediumvioletred);
    g.drawVerticalLine(bajoX, top, bottom);

    auto altoX = mapaX(altoParametroPtr->get());
    g.drawVerticalLine(altoX, top, bottom);


    auto mapaY = [bottom,top](float db)
    {
        return jmap(db, INFINITO_NEGATIVO, DECIBELIOS_MAXIMO, float(bottom), float(top));

    };

    auto ceroDb = mapaY(0.f);

    g.setColour(Colours::seagreen.withAlpha(0.3f));
    g.fillRect(Rectangle<float>::leftTopRightBottom(izquierda, ceroDb, bajoX, mapaY(bandaBajoGR)));
    g.fillRect(Rectangle<float>::leftTopRightBottom(bajoX, ceroDb, altoX, mapaY(bandaMedioGR)));
    g.fillRect(Rectangle<float>::leftTopRightBottom(altoX, ceroDb, derecha, mapaY(bandaAltoGR)));


    g.setColour(Colours::honeydew);
    g.drawHorizontalLine(mapaY(bajoLimiteParametroPtr->get()), izquierda, bajoX);
    g.drawHorizontalLine(mapaY(medioLimiteParametroPtr->get()), bajoX, altoX);
    g.drawHorizontalLine(mapaY(altoLimiteParametroPtr->get()), altoX, derecha);


}

void ComponenteAnalizador::actualiza(const std::vector<float>& valores)
{
    jassert(valores.size() == 6);
    enum
    {
        bandaBajoEntrada,
        bandaBajoSalida,
        bandaMedioEntrada,
        bandaMedioSalida,
        bandaAltoEntrada,
        bandaAltoSalida
    };

    bandaBajoGR = valores[bandaBajoSalida] - valores[bandaBajoEntrada];
    bandaMedioGR = valores[bandaMedioSalida] - valores[bandaMedioEntrada];
    bandaAltoGR = valores[bandaAltoSalida] - valores[bandaAltoEntrada];

    repaint();
}

std::vector<float> ComponenteAnalizador::getFrequencies()
{
    return std::vector<float>
    {
        20, /*30, 40,*/ 50, 100,
            200, /*300, 400,*/ 500, 1000,
            2000, /*3000, 4000,*/ 5000, 10000,
            20000
    };

}

std::vector<float> ComponenteAnalizador::getGains()
{
    std::vector<float> valores;

    auto incremento = DECIBELIOS_MAXIMO;
    for (auto db = INFINITO_NEGATIVO; db <= DECIBELIOS_MAXIMO; db += incremento)
    {
        valores.push_back(db);
    }

    return valores;
}

std::vector<float> ComponenteAnalizador::getXs(const std::vector<float>& freqs, float left, float width)
{
    std::vector<float> xs;
    for (auto f : freqs)
    {
        auto normX = juce::mapFromLog10(f, FRECUENCIA_MINIMA, FRECUENCIA_MAXIMA);
        xs.push_back(left + width * normX);
    }

    return xs;
}

void ComponenteAnalizador::drawBackgroundGrid(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    using namespace juce;
    auto freqs = getFrequencies();

    auto renderArea = getAnalysisArea(bounds);
    auto left = renderArea.getX();
    auto right = renderArea.getRight();
    auto top = renderArea.getY();
    auto bottom = renderArea.getBottom();
    auto width = renderArea.getWidth();

    auto xs = getXs(freqs, left, width);

    g.setColour(Colours::dimgrey);
    for (auto x : xs)
    {
        g.drawVerticalLine(x, top, bottom);
    }

    auto gain = getGains();

    for (auto gDb : gain)
    {
        // auto y = jmap(gDb, -24.f, 24.f, float(bottom), float(top));
        auto y = jmap(gDb, INFINITO_NEGATIVO, DECIBELIOS_MAXIMO, float(bottom), float(top));

        g.setColour(gDb == 0.f ? Colour(0u, 172u, 1u) : Colours::darkgrey);
        g.drawHorizontalLine(y, left, right);
    }
}

void ComponenteAnalizador::drawTextLabels(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    using namespace juce;
    g.setColour(Colours::lightgrey);
    const int fontHeight = 10;
    g.setFont(fontHeight);

    auto renderArea = getAnalysisArea(bounds);
    auto left = renderArea.getX();

    auto top = renderArea.getY();
    auto bottom = renderArea.getBottom();
    auto width = renderArea.getWidth();

    auto freqs = getFrequencies();
    auto xs = getXs(freqs, left, width);

    for (int i = 0; i < freqs.size(); ++i)
    {
        auto f = freqs[i];
        auto x = xs[i];

        bool addK = false;
        String str;
        if (f > 999.f)
        {
            addK = true;
            f /= 1000.f;
        }

        str << f;
        if (addK)
            str << "k";
        str << "Hz";

        auto textWidth = g.getCurrentFont().getStringWidth(str);

        Rectangle<int> r;

        r.setSize(textWidth, fontHeight);
        r.setCentre(x, 0);
       // r.setY(1);
        r.setY(bounds.getY());

        g.drawFittedText(str, r, juce::Justification::centred, 1);
    }

    auto gain = getGains();

    for (auto gDb : gain)
    {
       // auto y = jmap(gDb, -24.f, 24.f, float(bottom), float(top));
        auto y = jmap(gDb, INFINITO_NEGATIVO, DECIBELIOS_MAXIMO, (float)bottom, (float)top);
        String str;
        if (gDb > 0)
            str << "+";
        str << gDb;

        auto textWidth = g.getCurrentFont().getStringWidth(str);

        Rectangle<int> r;
        r.setSize(textWidth, fontHeight);
        r.setX(bounds.getRight() - textWidth);
        r.setCentre(r.getCentreX(), y);

        g.setColour(gDb == 0.f ? Colour(0u, 172u, 1u) : Colours::lightgrey);

        g.drawFittedText(str, r, juce::Justification::centredLeft, 1);

       
       

        r.setX(bounds.getX() + 1);
        g.drawFittedText(str, r, juce::Justification::centredLeft, 1);
    }
}

void ComponenteAnalizador::resized()
{
    using namespace juce;
    //LMP
    auto bounds = getLocalBounds();

    auto fftBounds = getAnalysisArea(bounds).toFloat();
    auto negativoInfinito = jmap(bounds.toFloat().getBottom(), fftBounds.getBottom(), fftBounds.getY(), INFINITO_NEGATIVO, DECIBELIOS_MAXIMO);

    DBG("Negativo Infinito: " << negativoInfinito);
    leftPathProducer.actualizaNegativoInfinito(negativoInfinito);
    rightPathProducer.actualizaNegativoInfinito(negativoInfinito);

}

void ComponenteAnalizador::parameterValueChanged(int parameterIndex, float newValue)
{
    parametersChanged.set(true);
}


void ComponenteAnalizador::timerCallback()
{
    if (shouldShowFFTAnalysis)
    {
        auto bounds = getLocalBounds();
        auto fftBounds = getAnalysisArea(bounds
   ).toFloat();
        fftBounds.setBottom(bounds.getBottom());
        auto sampleRate = audioProcessor.getSampleRate();

        leftPathProducer.process(fftBounds, sampleRate);
        rightPathProducer.process(fftBounds, sampleRate);
    }

    if (parametersChanged.compareAndSetBool(false, true))
    {

    }

    repaint();
}

juce::Rectangle<int> ComponenteAnalizador::getRenderArea(juce::Rectangle<int> bounds)
{

    bounds.removeFromTop(12);
    bounds.removeFromBottom(2);
    bounds.removeFromLeft(20);
    bounds.removeFromRight(20);

    return bounds;
}


juce::Rectangle<int> ComponenteAnalizador::getAnalysisArea(juce::Rectangle<int> bounds)
{
    bounds = getRenderArea(bounds);
    bounds.removeFromTop(4);
    bounds.removeFromBottom(4);
    return bounds;
}

