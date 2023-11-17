#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;

const int resolutionX = 960;
const int resolutionY = 960;
const int mushroom_size = 32;

bool isIntersecting(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) 
{
    // Calculate the right and bottom coordinates for each rectangle
    int first_right = x1 + w1;
    int first_bottom = y1 + h1;

    int second_right = x2 + w2;
    int second_bottom = y2 + h2;

    // If one rectangle is on left side of other
    if (first_right <= x2 || second_right <= x1) {
        return false;
    }

    // If one rectangle is above other
    if (first_bottom <= y2 || second_bottom <= y1) {
        return false;
    }

    // Rectangles overlap
    return true;
}

int main()
{


    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede");


    // Background
    sf::Texture backgroundTexture;
    if(!backgroundTexture.loadFromFile("background.png")) {
        cerr << "Error in loading background file. Terminating...\n";
        return -1;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    //Player
    sf::Texture playerTexture;
    if(!playerTexture.loadFromFile("player.png"))
    {
        cerr << "Error in loading player file. Terminating...\n";
        return -1;
    }
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setPosition(resolutionX/2 - playerTexture.getSize().x, resolutionY - playerTexture.getSize().y);

    //Bullet Creation
    sf::Texture bulletTexture;
    if(!bulletTexture.loadFromFile("bullet.png"))
    {
        cerr << "Error in loading bullet file. Terminating...\n";
        return -1;
    }
    sf::Sprite bulletSprite;
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setPosition(playerSprite.getPosition().x, playerSprite.getPosition().y - bulletTexture.getSize().y);
    bool bullet_visible = false;
    float bullet_speed = 1;

    //Mushroom Creation
    sf::Texture mushroomTexture;
    if(!mushroomTexture.loadFromFile("mushroom.png"))
    {
        cerr << "Error in loading mushroom file. Terminating...\n";
        return -1;
    }
    sf::Sprite mushroom;
    mushroom.setTexture(mushroomTexture);
    mushroom.setTextureRect(sf::IntRect(0, 0, mushroom_size, mushroom_size));
    mushroom.setPosition((rand() % resolutionX + 1), (rand() % resolutionY + 1));
    bool mushroom_visible = true;
    int mushroom_hits = 0;

    //Score
    int score = 0;


    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // window.clear(sf::Color::Black);
        window.draw(backgroundSprite);

        // draw everything here...


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && playerSprite.getPosition().x > 0)
        {
            playerSprite.move(-1, 0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && playerSprite.getPosition().x < resolutionX - playerSprite.getTexture()->getSize().x)
        {
            playerSprite.move(1, 0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && playerSprite.getPosition().y > 800)
        {
            playerSprite.move(0, -1);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && playerSprite.getPosition().y < resolutionY - playerSprite.getTexture()->getSize().y)
        {
            playerSprite.move(0, 1);
        }
        window.draw(playerSprite);

        if(mushroom_visible)
        {
            window.draw(mushroom);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) && !bullet_visible)
        {
            bulletSprite.setPosition(playerSprite.getPosition().x, playerSprite.getPosition().y - bulletTexture.getSize().y);
            bullet_visible = true;
        }

        if(bullet_visible)
        {   
            window.draw(bulletSprite);
            bulletSprite.setPosition(bulletSprite.getPosition().x, bulletSprite.getPosition().y - bullet_speed);
            if(bulletSprite.getPosition().y < -static_cast<int>(bulletTexture.getSize().y))
            {
                bullet_visible = false;
            }
            if(mushroom_visible && isIntersecting(bulletSprite.getPosition().x, bulletSprite.getPosition().y, bulletTexture.getSize().x,
                 bulletTexture.getSize().y, mushroom.getPosition().x, mushroom.getPosition().y, mushroom_size, mushroom_size))
            {
                
                cout << "Boom\n";
                bullet_visible = false;
                mushroom_hits++;
                if(mushroom_hits == 2)
                {
                    mushroom_visible = false;
                    score++;
                    cout << "Score = " << score << "\n";
                }

            }
        }

        // end the current frame
        window.display();
    }
    return 0;
}