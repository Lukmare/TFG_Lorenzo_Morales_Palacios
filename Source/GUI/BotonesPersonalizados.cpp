/*
  ==============================================================================

    BotonesPersonalizados.cpp
    Created: 5 May 2023 3:02:00pm
    Author:  loren

  ==============================================================================
*/

#include "BotonesPersonalizados.h"

void BotonDelAnalizador::resized()
{
    auto bounds = getLocalBounds();
    auto insertaRectangulo = bounds.reduced(4);

    pathAleatorio.clear();

    juce::Random r;

    pathAleatorio.startNewSubPath(insertaRectangulo.getX(),
        insertaRectangulo.getY() + insertaRectangulo.getHeight() * r.nextFloat());

    for (auto x = insertaRectangulo.getX() + 1; x < insertaRectangulo.getRight(); x += 2)
    {
        pathAleatorio.lineTo(x,
            insertaRectangulo.getY() + insertaRectangulo.getHeight() * r.nextFloat());
    }
}