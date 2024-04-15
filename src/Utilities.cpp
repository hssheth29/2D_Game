#include "../include/Utilities.h"

void text_setup(sf::Text* text, sf::Color fill, sf::Color outline)
{
    text->setFillColor(fill);
    text->setOutlineColor(outline);
    text->setOutlineThickness(1.f);
    text->setLetterSpacing(1.f);
    text->setStyle(sf::Text::Italic);
}

void bgmusic_setup(sf::Music* music, const std::string& file)
{
    music->openFromFile(file);
    music->setLoop(true);
    music->setPitch(float(1.05));
    music->setVolume(20);
}