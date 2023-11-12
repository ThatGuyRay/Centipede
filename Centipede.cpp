#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;

const int resolutionX = 960;
const int resolutionY = 960;

int main()
{
    sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede");

    sf::RectangleShape player(sf::Vector2f(30, 30));
    player.setFillColor(sf::Color::Red);
    player.setPosition(385, 570);

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
        window.clear(sf::Color::Black);

        // draw everything here...


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && player.getPosition().x > 0)
        {
            player.move(-1, 0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && player.getPosition().x < resolutionX - player.getSize().x)
        {
            player.move(1, 0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && player.getPosition().y > 450)
        {
            player.move(0, -1);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && player.getPosition().y < resolutionY - player.getSize().y)
        {
            player.move(0, 1);
        }
        window.draw(player);

        

        // end the current frame
        window.display();
    }
    return 0;
}