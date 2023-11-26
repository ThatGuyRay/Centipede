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
const int playerRows = 5;

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

const int maxCentipedeSize = 12;

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void movePlayer(float player[]);
void moveBullet(float bullet[]);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
void fireBullet(float bullet[], float player[]);
void drawMushrooms(sf::RenderWindow& window, sf::Texture& mushroomTexture);
void gridToPixel(int gridx, int gridy, int pixelarray[]);
void generateMushrooms(int number_of_mushrooms);
bool bulletCollision(float bullet[], int& score);
void pixelToGrid(float pixelarray[], int gridarray[]);
void generateCentipede(int len, bool isleft, int headposition[], int& numberofCentipede, 
    bool isLeft[], int centipedeLength[], int centipedeheadPositionX[], int centipedeheadPositionY[]);
void drawCentipede(sf::RenderWindow& window, sf::Texture& centipedeheadTexture, sf::Texture& centipedebodyTexture, 
    int centipede_head_positionX[], int centipede_head_positionY[], int numberofCentipedes, int centipedeLength[maxCentipedeSize]);
void UpdateGrid(int numberofCentipede, bool isLeft[], int centipedeLength[], int centipedeheadPositionX[], int centipedeheadPositionY[]);
void moveCentipede(int numberofCentipede, bool isLeft[], int centipedeheadPositionX[], int centipedeheadPositionY[]);

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

    //Set up Centipedes.
    int numberofCentipede = 0;
    int centipedeLength[maxCentipedeSize] = {};
    bool isLeft[maxCentipedeSize] = {};
    int centipedeheadPositionX[maxCentipedeSize] = {};
    int centipedeheadPositionY[maxCentipedeSize] = {};

    int starting_position[2] = {5, 4};
    generateCentipede(12, true, starting_position, numberofCentipede, isLeft, centipedeLength, centipedeheadPositionX, centipedeheadPositionY);


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
    generateMushrooms(20 + (rand() % 10));

    //Centipede Head Creation
    sf::Texture centipedeheadTexture;
    if(!centipedeheadTexture.loadFromFile("c_head_left_walk.png"))
    {
        cerr << "Error in loading centipede head file. Terminating...\n";
        return -1;
    }

    //Centipede Body Creation
    sf::Texture centipedebodyTexture;
    if(!centipedebodyTexture.loadFromFile("c_body_left_walk.png"))
    {
        cerr << "Error in loading centipede body file. Termiating...\n";
        return -1;
    }

    //Score
    int score = 0;
    int centipederate = 0;

    sf::Clock frameRateClock;

    while (window.isOpen())
    {
        // check time elapsed
        if(frameRateClock.getElapsedTime().asSeconds() < 1.0f/frameRate)
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
            if(bulletCollision(bullet, score))
            {
                bullet[exists] = false;
            }
        }
        if(centipederate == 20)
        {
            moveCentipede(numberofCentipede, isLeft, centipedeheadPositionX, centipedeheadPositionY);
            centipederate = 0;
        }
        centipederate++;

        UpdateGrid(numberofCentipede, isLeft, centipedeLength, centipedeheadPositionX, centipedeheadPositionY);
        drawCentipede(window, centipedeheadTexture, centipedebodyTexture, centipedeheadPositionX, centipedeheadPositionY, numberofCentipede, centipedeLength);
        

        drawMushrooms(window, mushroomTexture);



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
    float speed = 10;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && player[x] > 0)
    {
        player[x] -= speed;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && player[x] < resolutionX - boxPixelsX)
    {
        player[x] += speed;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && player[y] > resolutionY - (playerRows * boxPixelsY))
    {
        player[y] -= speed;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && player[y] < resolutionY - boxPixelsY)
    {
        player[y] += speed; 
    }
}

void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}

void moveBullet(float bullet[])
{
    bullet[y] -= 20;	
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

void drawMushrooms(sf::RenderWindow& window, sf::Texture& mushroomTexture)
{
    for(int i = 0; i < gameRows; i++)
    {
        for(int j = 0; j < gameColumns; j++)
        {
            if(gameGrid[i][j] == 2 || gameGrid[i][j] == 1)
            {
                sf::Sprite mushroom;
                mushroom.setTexture(mushroomTexture);
                if(gameGrid[i][j] == 2)
                {
                    mushroom.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
                }
                else if(gameGrid[i][j] == 1)
                {
                    mushroom.setTextureRect(sf::IntRect(32, 0, boxPixelsX, boxPixelsY));
                }
                int positionArray[2] = {};
                gridToPixel(i, j, positionArray);
                mushroom.setPosition(positionArray[x], positionArray[y]);
                window.draw(mushroom);
            }
        }
    }
}

void gridToPixel(int gridx, int gridy, int pixelarray[])
{
    pixelarray[0] = gridx * 32;
    pixelarray[1] = gridy * 32;
}

void generateMushrooms(int number_of_mushrooms)
{
    int current_total = 0;
    while(current_total <= number_of_mushrooms)
    {
        int random_row = (rand() % gameRows);
        int random_column = (rand() % (gameColumns - playerRows)); // Leaving the last row empty of mushrooms
        if(gameGrid[random_row][random_column] == 0)
        {
            gameGrid[random_row][random_column] = 2;
            current_total++;
        }
    }

}

bool bulletCollision(float bullet[], int& score)
{
    int gridarray[2];
    int hitcounter = 0;
    pixelToGrid(bullet, gridarray);
    if(gameGrid[gridarray[0]][gridarray[1]] == 2 || gameGrid[gridarray[0]][gridarray[1]] == 1)
    {
        gameGrid[gridarray[0]][gridarray[1]]--;
        if(gameGrid[gridarray[0]][gridarray[1]] == 0)
        {
            score++;
            cout << "Score: " << score << endl;
        }
        return true;
    }
    else
    {
        return false;
    }
}

void pixelToGrid(float pixelarray[], int gridarray[])
{
    gridarray[0] = pixelarray[0] / 31;
    gridarray[1] = pixelarray[1] / 32;
}

void generateCentipede(int len, bool isleft, int headposition[], int& numberofCentipede, 
    bool isLeft[], int centipedeLength[], int centipedeheadPositionX[], int centipedeheadPositionY[])
{
    centipedeheadPositionX[numberofCentipede] = headposition[x];
    centipedeheadPositionY[numberofCentipede] = headposition[y];
    centipedeLength[numberofCentipede] = len;
    isLeft[numberofCentipede] = isleft;
    numberofCentipede++;
}

void drawCentipede(sf::RenderWindow& window, sf::Texture& centipedeheadTexture, sf::Texture& centipedebodyTexture,
    int centipede_head_positionX[], int centipede_head_positionY[], int numberofCentipedes, int centipedeLength[maxCentipedeSize])
{
    for (int i = 0; i < gameRows; i++)
    {
        for (int j = 0; j < gameColumns; j++)
        {
            if (gameGrid[i][j] == 10 || gameGrid[i][j] == 11)
            {
                sf::Sprite centipedeSprite;
                if(gameGrid[i][j] == 10)
                {
                    centipedeSprite.setTexture(centipedeheadTexture);
                }
                if(gameGrid[i][j] == 11)
                {
                    centipedeSprite.setTexture(centipedebodyTexture);
                }
                centipedeSprite.setTextureRect(sf::IntRect(0, 0, 28, 32));
                int positionArray[2] = {};
                gridToPixel(i, j, positionArray);
                centipedeSprite.setPosition(positionArray[x], positionArray[y]);
                window.draw(centipedeSprite);
            }
                
        }
    }
}

void UpdateGrid(int numberofCentipede, bool isLeft[], int centipedeLength[], int centipedeheadPositionX[], int centipedeheadPositionY[])
{
    for(int i = 0; i < gameRows; i++)
    {
        for(int j = 0; j < gameColumns; j++)
        {
            if(gameGrid[i][j] == 10 || gameGrid[i][j] == 11)
            {
                gameGrid[i][j] = 0;
            }
        }
    }

    for(int i = 0; i < numberofCentipede; i++)
    {
        int currentx = centipedeheadPositionX[i];
        int currenty = centipedeheadPositionY[i];
        bool direction = isLeft[i];
        gameGrid[currentx][currenty] = 10;
        for(int j = 0; j < centipedeLength[i]; j++)
        {
            if(direction == false)
            {
                currentx -= 1;
                if(currentx < 0)
                {
                    currenty--;
                    currentx++;
                    direction = true;
                }
            }
            else
            {
                currentx += 1;
                if(currentx > gameColumns)
                {
                    currenty--;
                    currentx--;
                    direction = false;
                }
            }
            gameGrid[currentx][currenty] = 11;
        }
    }
}

void moveCentipede(int numberofCentipede, bool isLeft[], int centipedeheadPositionX[], int centipedeheadPositionY[])
{
    for(int i = 0; i < numberofCentipede; i++)
    {
        bool direction = isLeft[i];
        int currentx = centipedeheadPositionX[i];
        int currenty = centipedeheadPositionY[i];
        if(direction == true)
        {
            currentx -= 1;
            if(currentx < 0)
            {
                currentx=0;
                currenty++;
                direction = false;
            }
        }
        else if(direction == false)
        {
            currentx += 1;
            if(currentx > gameColumns - 1)
            {
                currentx = gameColumns - 1;
                currenty++;
                direction = true;
            }
        }
        if(gameGrid[currentx][currenty] != 0)
        {
            currenty++;
            currentx = direction ? currentx + 1 : currentx - 1;
            direction = !direction;
        }
        centipedeheadPositionX[i] = currentx;
        centipedeheadPositionY[i] = currenty;
        isLeft[i] = direction;
    }
}