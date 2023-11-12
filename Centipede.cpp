#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Centipede");

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
        sf::RectangleShape player(sf::Vector2f(30, 30));
        player.setFillColor(sf::Color::Red);
        player.setPosition(385, 570);
        window.draw(player);

        // end the current frame
        window.display();
    }
    return 0;
}