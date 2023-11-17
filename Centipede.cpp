#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;

const int resolutionX = 960;
const int resolutionY = 960;

int main()
{
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
    bulletSprite.setPosition(450, 450);

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
        window.draw(bulletSprite);

        

        // end the current frame
        window.display();
    }
    return 0;
}