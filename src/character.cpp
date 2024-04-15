#include "../include/character.h"
#include <iostream>

Character::Character():left(0), top(0), width(0), height(0), frameCount(1) {}

Character::Character(const std::string& filename, std::string n, int x, int y, int w, int h, int f):\
                                     left(x), top(y), width(w), height(h), frameCount(f)
{
    name = std::move(n);
    texture.loadFromFile(filename);
    texture.setSmooth(true);
    changePosture(x, y, w, h, f);
}

void Character::changePosture(int x, int y, int w, int h, int f)
{
    std::vector<sf::IntRect> animFrames;
    int i = 0;
    while (i < f)
    {
        animFrames.emplace_back(x + (w * i), y, w, h);
        i++;
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(animFrames[0]);
}

void Character::checkBoundaryCollision(sf::FloatRect bounds)
{
    sf::Vector2f position = sprite.getPosition();
    position.x = std::max(position.x, bounds.left);
    position.y = std::max(position.y, bounds.top);
    sprite.setPosition(position);
    position.x = std::min(position.x, bounds.width - width);
    position.y = std::min(position.y, bounds.height - height);
    sprite.setPosition(position);
}

void Character::checkObjectCollision(Character* a, Character* b, sf::Vector2f &v)
{
    //Collision detection - Player vs Enemy/Wall/Key(2D obj vs 2D obj)
    sf::FloatRect aBounds = a->sprite.getGlobalBounds();
    sf::FloatRect bBounds = b->sprite.getGlobalBounds();
    aBounds.left += v.x;
    aBounds.top += v.y;
    if (aBounds.intersects(bBounds))
    {
        //right wall
        if (aBounds.left < bBounds.left &&
            aBounds.left + aBounds.width < bBounds.left + bBounds.width &&
            aBounds.top < bBounds.top + bBounds.height &&
            aBounds.top + aBounds.height > bBounds.top)
        {
            a->sprite.setPosition(bBounds.left - aBounds.width, aBounds.top);
        }
            //left wall
        else if (aBounds.left > bBounds.left &&
                 aBounds.left + aBounds.width > bBounds.left + bBounds.width &&
                 aBounds.top < bBounds.top + bBounds.height &&
                 aBounds.top + aBounds.height > bBounds.top)
        {
            a->sprite.setPosition(bBounds.left + bBounds.width, aBounds.top);
        }
            //bottom wall
        else if (aBounds.top < bBounds.top &&
                 aBounds.top + aBounds.height < bBounds.top + bBounds.height &&
                 aBounds.left < bBounds.left + bBounds.width &&
                 aBounds.left + aBounds.width > bBounds.left)
        {
            a->sprite.setPosition(aBounds.left, bBounds.top - aBounds.height);
        }
            //top wall
        else if (aBounds.top > bBounds.top &&
                 aBounds.top + aBounds.height > bBounds.top + bBounds.height &&
                 aBounds.left < bBounds.left + bBounds.width &&
                 aBounds.left + aBounds.width > bBounds.left)
        {
            a->sprite.setPosition(aBounds.left, bBounds.top + aBounds.height - 40.f);
        }
        if (b->name == "cannonball")
        {
            if (b->notCollided)
            {
                a->score -= 2000;
                std::cout << "Debug: Ouch!" << std::endl;
            }
            b->notCollided = false;
        }
        if (b->name == "key")
        {
            if (b->notCollided)
            {
                a->score += 2000;
                std::cout << "Debug: Hurray!" << std::endl;
            }
            b->notCollided = false;
        }
    }
}

void Character::movementSetting(float s, int i, int sf)
{
    step = 1;
    speed = s;
    interval = i;
    startFrame = sf;
}