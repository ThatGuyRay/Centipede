#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid
const int frameRate = 60;

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void movePlayer(float player[]);
void moveBullet(float bullet[]);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
void fireBullet(float bullet[], float player[]);



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

    // Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	// window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

    // Todo: Add Music.



    // Background
    sf::Texture backgroundTexture;
    if(!backgroundTexture.loadFromFile("background.png")) {
        cerr << "Error in loading background file. Terminating...\n";
        return -1;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.50)); // Reduce Opacity to 25%

    // Initializing Player and Player Sprites.
	float player[2] = {};
    player[x] = resolutionX/2 - boxPixelsX;
    player[y] = resolutionY - boxPixelsY;
    sf::Texture playerTexture;
    if(!playerTexture.loadFromFile("player.png"))
    {
        cerr << "Error in loading player file. Terminating...\n";
        return -1;
    }
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
   
    // Initializing Bullet and Bullet Sprites.
    float bullet[3] = {};
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = false;
    sf::Texture bulletTexture;
    if(!bulletTexture.loadFromFile("bullet.png"))
    {
        cerr << "Error in loading bullet file. Terminating...\n";
        return -1;
    }
    sf::Sprite bulletSprite;
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    //Mushroom Creation
    sf::Texture mushroomTexture;
    if(!mushroomTexture.loadFromFile("mushroom.png"))
    {
        cerr << "Error in loading mushroom file. Terminating...\n";
        return -1;
    }
    sf::Sprite mushroom;
    mushroom.setTexture(mushroomTexture);
    mushroom.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
    mushroom.setPosition((rand() % resolutionX + 1), (rand() % resolutionY + 1));
    bool mushroom_visible = true;
    int mushroom_hits = 0;

    //Score
    int score = 0;

    sf::Clock frameRateClock;

    while (window.isOpen())
    {
        // check time elapsed
        if(frameRateClock.getElapsedTime().asSeconds() < 1/frameRate)
        {
            continue;
        }
        frameRateClock.restart();
        
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        window.draw(backgroundSprite);

        // draw everything here...
        movePlayer(player);
        drawPlayer(window, player, playerSprite);

        fireBullet(bullet, player);

        if (bullet[exists] == true) 
        {
			moveBullet(bullet);
			drawBullet(window, bullet, bulletSprite);
        }


        if(mushroom_visible)
        {
            window.draw(mushroom);
        }

        // if(bullet_visible)
        // {   
        //     window.draw(bulletSprite);
        //     bulletSprite.setPosition(bulletSprite.getPosition().x, bulletSprite.getPosition().y - bullet_speed);
        //     if(bulletSprite.getPosition().y < -static_cast<int>(bulletTexture.getSize().y))
        //     {
        //         bullet_visible = false;
        //     }
        //     if(mushroom_visible && isIntersecting(bulletSprite.getPosition().x, bulletSprite.getPosition().y, bulletTexture.getSize().x,
        //          bulletTexture.getSize().y, mushroom.getPosition().x, mushroom.getPosition().y, boxPixelsX, boxPixelsY))
        //     {
                
        //         cout << "Boom\n";
        //         bullet_visible = false;
        //         mushroom_hits++;
        //         if(mushroom_hits == 2)
        //         {
        //             mushroom_visible = false;
        //             score++;
        //             cout << "Score = " << score << "\n";
        //         }

        //     }
        // }

        // end the current frame
        window.display();
    }
    return 0;
}

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}

void movePlayer(float player[])
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && player[x] > 0)
    {
        player[x] -= 1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && player[x] < resolutionX - boxPixelsX)
    {
        player[x] += 1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && player[y] > 800)
    {
        player[y] -= 1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && player[y] < resolutionY - boxPixelsY)
    {
        player[y] += 1; 
    }
}

void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}

void moveBullet(float bullet[])
{
    bullet[y] -= 1;	
	if (bullet[y] < -32)
    {
		bullet[exists] = false;
    }
}

void fireBullet(float bullet[], float player[])
{
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) && !bullet[exists])
        {
            bullet[x] = player[x];
            bullet[y] = player[y] - boxPixelsY;
            bullet[exists] = true;
        }
}
