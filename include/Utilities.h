#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

void text_setup(sf::Text* text, sf::Color fill, sf::Color outline);
void bgmusic_setup(sf::Music* music, const std::string& file);

#endif