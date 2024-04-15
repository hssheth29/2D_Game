#include "../include/character.h"
#include "../include/Utilities.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>

using namespace std;
using namespace sf;

// Global variables and constants
unsigned int xWin = 1280, yWin = 600;
unsigned int FRLimit = 60;
string playerAnim = "assets/images/pirate_motion.png";
string wallAnim = "assets/images/wall.png";
string keyAnim = "assets/images/gazebo_key.png";
string iconImage = "assets/images/pirateicon64.png";
string enemyAnim = "assets/images/cannonball.png";
string fontFile = "assets/fonts/clarenbd.ttf";
string bgmusicFile = "assets/music/bgmusic.ogg";
string golaughFile = "assets/music/gameover_laugh.ogg";

int main()
{
    //Window
    RenderWindow window(VideoMode(xWin, yWin), "2D SFML GAME", Style::Default);
    window.setFramerateLimit(FRLimit);
    FloatRect windowBounds(Vector2f(0.f, 0.f), window.getDefaultView().getSize());

    //Set Icon
    Image icon;
    icon.loadFromFile(iconImage);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    //Create game entities
    Character player(playerAnim, "player", 0, 0, 55, 90);
    player.sprite.setPosition(0.f, float(yWin - 90));
    vector<vector<int>>playerPostures{ {0, 0, 60, 90, 1}, {60, 0, 55, 90, 1}, {225, 0, 55, 90, 1} };
    int posture = 0;

    Character key(keyAnim, "key", 0, 0, 110, 140);
    key.sprite.setPosition(float(xWin) - 110.f, 0.f);

    list<Character> walls;
    vector<vector<float>>wallCoverage{ \
                        {44.f, 0.f, 600.f, 50.f,-5.f, 1.f}, {534.f, 0.f, 616.f, 50.f,700.f,1.f }, \
                        {644.f, 0.f, 308.f,50.f,-5.f,2.f}, {309.f, 0.f, 616.f,50.f,400.f,2.f}, {670.f, 0.f, 200.f,50.f,1100.f,2.f}, \
                        {100.f, 0.f, 1100.f,50.f,-5.f, 3.f} };
    for (auto & i : wallCoverage)
    {
        Character block(wallAnim, "wall", int(i[0]), int(i[1]),\
                        int(i[2]), int(i[3]));
        block.sprite.setPosition(float(i[4]), \
                                 float(yWin - (50 + 90) * i[5] - (50.f * i[5])));
        walls.push_back(block);
    }

    list<Character> enemies;
    vector<vector<float>> enemyStartPosition{ {float(xWin - 40), float(yWin - 40), -2.f, 10.f, 320.f},\
                                              {0.f, float(yWin - 40), 1.5, 15.f, 40.f}, \
                                              {float(xWin - 40), float(yWin - 40), -2.f, 20.f, 320.f}, \
                                              {float(xWin + 300 - 40), float(yWin - 90), -2.f, 10.f, 320.f}, \
                                              {-300.f, float(yWin - 90), 1.5, 15.f, 40.f}, \
                                              {float(xWin+300 - 40), float(yWin - 90), -2.f, 20.f, 320.f} };
    for (unsigned int i = 0; i < enemyStartPosition.size(); i++)
    {
        Character cannonball(enemyAnim, "cannonball", 0, 0, 40, 40);
        cannonball.sprite.setPosition(float(enemyStartPosition[i][0]), float(enemyStartPosition[i][1] - i % 3 * (90 + 100)));
        cannonball.movementSetting(enemyStartPosition[i][2], int(enemyStartPosition[i][3]), int(enemyStartPosition[i][4]));
        enemies.push_back(cannonball);
    }

    Event event{};
    float moveUnitMin, moveUnitMax;
    moveUnitMin = -2, moveUnitMax = 2;

    Font font;
    font.loadFromFile(fontFile);
    string scoretext = "  SCORE: ";
    string lifetext = "  LIFE: 1";
    string scoreboard = lifetext + scoretext + to_string(player.score);
    Text Score(scoreboard, font, 20);
    text_setup(&Score, Color(200, 90, 90, 255), Color::Black);

    Music music;
    bgmusic_setup(&music, bgmusicFile);
    music.play();

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                cout << "Debug: Score: " << player.score;
                window.close();
            }
        }
        Vector2f velocity(0.f, 0.f);
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            velocity.x = moveUnitMax;
            posture = 1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            velocity.x = moveUnitMin;
            posture = 2;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            velocity.y = moveUnitMin;
            if (posture == 1)
                posture = 1;
            else if (posture == 2)
                posture = 2;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            velocity.y = moveUnitMax;
            if (posture == 1)
                posture = 1;
            else if (posture == 2)
                posture = 2;
        }

        player.sprite.move(velocity.x, velocity.y);
        player.changePosture(playerPostures[posture][0], playerPostures[posture][1], \
                             playerPostures[posture][2], playerPostures[posture][3], \
                             playerPostures[posture][4]);

        player.score += int(abs(velocity.x) + abs(velocity.y));
        player.checkBoundaryCollision(windowBounds);

        for (auto wall : walls)
        {
            Character::checkObjectCollision(&player, &wall, velocity);
        }

        Character::checkObjectCollision(&player, &key, velocity);

        for (auto& enemy : enemies)
        {
            if (enemy.notCollided)
            {
                enemy.sprite.move(enemy.speed, 0.f);
                if (int(enemy.sprite.getGlobalBounds().left) % enemy.interval == 0)
                {
                    if (enemy.step < 9)
                    {
                        enemy.changePosture(abs(enemy.startFrame - 40 * enemy.step), 0, 40, 40, 1);
                        enemy.step++;
                    }
                }
                if (enemy.step == 9)
                    enemy.step = 1;
            }
            Character::checkObjectCollision(&player, &enemy, velocity);
        }

        int outofbound = 0;
        for (auto& enemy : enemies)
        {
            float xval = enemy.sprite.getPosition().x;
            if (xval < 0 || xval > xWin || !enemy.notCollided)
            {
                outofbound++;
            }
        }
        if (outofbound==6)
        {
            enemies.clear();
            for (unsigned int i = 0; i < enemyStartPosition.size(); i++)
            {
                Character cannonball(enemyAnim, "cannonball", 0, 0, 40, 40);
                cannonball.sprite.setPosition(float(enemyStartPosition[i][0]), float(enemyStartPosition[i][1] - i % 3 * (90 + 100)));
                cannonball.movementSetting(enemyStartPosition[i][2], int(enemyStartPosition[i][3]), int(enemyStartPosition[i][4]));
                enemies.push_back(cannonball);
            }
        }

        scoreboard = lifetext + scoretext + to_string(player.score);
        Score.setString(scoreboard);

        Character::checkObjectCollision(&player, &key, velocity);
        if(player.score > 1000 && !key.notCollided && \
           player.sprite.getGlobalBounds().left < xWin-key.sprite.getGlobalBounds().width && \
           player.sprite.getGlobalBounds().top < key.sprite.getGlobalBounds().height)
        {
            music.stop();
            window.draw(Score);
            Music laugh;
            bgmusic_setup(&laugh, golaughFile);
            laugh.play();
            RenderWindow gameover(VideoMode(xWin/5, yWin/5), "Game Over!", Style::Close);
            gameover.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
            while (gameover.isOpen())
            {
                while (gameover.pollEvent(event))
                {
                    if (event.type == Event::Closed)
                    {
                        gameover.close();
                        window.close();
                    }
                }
                Text Message("You won!", font, 25);
                Message.setPosition(70.f, gameover.getSize().y/2.f-25.f);
                text_setup(&Message, Color::Black, Color::White);
                gameover.clear(Color(183, 50, 57, 0));
                gameover.draw(Message);
                gameover.display();
            }
        }

        window.clear(Color(30, 30, 40,255));
        window.draw(player.sprite);
        for(auto wall: walls)
            window.draw(wall.sprite);
        window.draw(key.sprite);
        for (auto enemy : enemies)
            window.draw(enemy.sprite);
        window.draw(Score);
        window.display();
    }
    return 0;
}