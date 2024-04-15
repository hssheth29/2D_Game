#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Character {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    int left, top, width, height;
    int frameCount;
    std::string name;
    bool notCollided = true;
    int score = 0;
    int step = 1;
    float speed = 1.f;
    int interval = 10;
    int startFrame = 40;

    Character();
    Character(const std::string& filename, std::string n, int x = 0, int y = 0, int w = 0, int h = 0, int f = 1);
    void changePosture(int x, int y, int w, int h, int f);
    void checkBoundaryCollision(sf::FloatRect bounds);
    static void checkObjectCollision(Character* a, Character* b, sf::Vector2f &v);
    void movementSetting(float s, int i, int sf);
};

#endif