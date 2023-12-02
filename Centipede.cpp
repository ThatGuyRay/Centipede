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
// void generateCentipede(int len, bool isleft, int headposition[], int& numberofCentipede, 
//     bool isLeft[], int centipedeLength[], int centipedeheadPositionX[], int centipedeheadPositionY[]);
void drawCentipede(sf::RenderWindow& window, sf::Texture& centipedeheadTexture, sf::Texture& centipedebodyTexture, 
    int maxnumber_centipede, int numberofcentipedes, int centipede[][maxCentipedeSize][2]);
void UpdateGrid(int numberofCentipede, int centipede[][maxCentipedeSize][2]);

void moveCentipede(int numberofCentipede, bool isLeft[], bool isBottom[], int centipede[][maxCentipedeSize][2]);

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
    int maxnumber_centipede = 12;
    // int centipedeLength[maxCentipedeSize] = {};
    bool isLeft[maxCentipedeSize] = {};
    bool isBottom[maxCentipedeSize] = {};
    // int centipedeheadPositionX[maxCentipedeSize] = {};
    // int centipedeheadPositionY[maxCentipedeSize] = {};
    int centipedearray[maxnumber_centipede][maxCentipedeSize][2] = {};

    //Head
    centipedearray[0][0][x] = 5;
    centipedearray[0][0][y] = 4;
    //Body-1
    centipedearray[0][1][x] = 4;
    centipedearray[0][1][y] = 4;
    //Body-2
    centipedearray[0][2][x] = 3;
    centipedearray[0][2][y] = 4;
    //Body-3
    centipedearray[0][3][x] = 2;
    centipedearray[0][3][y] = 4;
    //Body-4
    centipedearray[0][4][x] = 1;
    centipedearray[0][4][y] = 4;
    //Body-5
    centipedearray[0][5][x] = 0;
    centipedearray[0][5][y] = 4;
    //Body-6
    centipedearray[0][6][x] = 0;
    centipedearray[0][6][y] = 3;
    //Body-7
    centipedearray[0][7][x] = 1;
    centipedearray[0][7][y] = 3;
    //Body-8
    centipedearray[0][8][x] = 2;
    centipedearray[0][8][y] = 3;
    //Body-9
    centipedearray[0][9][x] = 3;
    centipedearray[0][9][y] = 3;
    //Body-10
    centipedearray[0][10][x] = 4;
    centipedearray[0][10][y] = 3;
    //Body-11
    centipedearray[0][11][x] = 5;
    centipedearray[0][11][y] = 3;
    isLeft[0] = false;
    isBottom[0] = true;

    numberofCentipede++;

    // int starting_position[2] = {5, 4};
    // generateCentipede(12, true, starting_position, numberofCentipede, isLeft, centipedeLength, centipedeheadPositionX, centipedeheadPositionY);


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
        if(centipederate == 2)
        {
            moveCentipede(numberofCentipede, isLeft, isBottom, centipedearray);
            centipederate = 0;
        }
        centipederate++;

        UpdateGrid(numberofCentipede, centipedearray);    
        drawCentipede(window, centipedeheadTexture, centipedebodyTexture, maxnumber_centipede, numberofCentipede, centipedearray);
        

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

void drawCentipede(sf::RenderWindow& window, sf::Texture& centipedeheadTexture, sf::Texture& centipedebodyTexture, 
    int maxnumber_centipede, int numberofcentipedes, int centipede[][maxCentipedeSize][2])
{
    for(int currentcentipede = 0; currentcentipede < numberofcentipedes; currentcentipede++)
    {
        for(int bodysegment = 0; bodysegment < maxCentipedeSize; bodysegment++)
        {
            if(centipede[currentcentipede][bodysegment][x] == -1)
            {
                break;
            }

            int bodyx = centipede[currentcentipede][bodysegment][x];
            int bodyy = centipede[currentcentipede][bodysegment][y];

            sf::Sprite centipedeSprite;
            if(bodysegment == 0)
            {
                centipedeSprite.setTexture(centipedeheadTexture);
            }
            else 
            {
                centipedeSprite.setTexture(centipedebodyTexture);
            }
            centipedeSprite.setTextureRect(sf::IntRect(0, 0, 28, 32));
            int positionArray[2] = {};
            gridToPixel(bodyx, bodyy, positionArray);
            centipedeSprite.setPosition(positionArray[x], positionArray[y]);
            window.draw(centipedeSprite);
        }
    }
}

void UpdateGrid(int numberofCentipede, int centipede[][maxCentipedeSize][2]) {
    // Delete all old centipede segments from the game grid
    for (int cI = 0; cI < numberofCentipede; cI++) {
        for (int bI = 0; bI < maxCentipedeSize; bI++) {
            int bx = centipede[cI][bI][x];
            int by = centipede[cI][bI][y];
            if (bx >= 0 && by >= 0 && bx < gameRows && by < gameColumns) {
                gameGrid[bx][by] = 0;
            }
        }
    }

    // Add new centipede segments to the game grid
    for (int cI = 0; cI < numberofCentipede; cI++) {
        for (int bI = 0; bI < maxCentipedeSize; bI++) {
            int bx = centipede[cI][bI][x];
            int by = centipede[cI][bI][y];
            if (bx >= 0 && by >= 0 && bx < gameRows && by < gameColumns) {
                gameGrid[bx][by] = (bI == 0) ? 10 : 11;
            }
        }
    }
}


void moveCentipede(int numberofCentipede, bool isLeft[], bool isBottom[], int centipede[][maxCentipedeSize][2])
{
    for(int currentcentipede = 0; currentcentipede < numberofCentipede; currentcentipede++)
    {
        for(int bodysegment = maxCentipedeSize - 1; bodysegment > 0; bodysegment--)
        {
            if(centipede[currentcentipede][bodysegment][x] == -1)
            {
                continue;
            }

            centipede[currentcentipede][bodysegment][x] = centipede[currentcentipede][bodysegment - 1][x];
            centipede[currentcentipede][bodysegment][y] = centipede[currentcentipede][bodysegment - 1][y];
        }
        
        int currentx = centipede[currentcentipede][0][x];
        int currenty = centipede[currentcentipede][0][y];
        bool xdirection = isLeft[currentcentipede];
        bool ydirection = isBottom[currentcentipede];

        currentx = xdirection ? currentx - 1 : currentx + 1;
        // at the edge
        if (currentx < 0 || currentx > gameColumns - 1) {
            if (currentx < 0) {
                currentx = 0;
            } else {
                currentx = gameColumns - 1;
            }
            xdirection = !xdirection;
            // check if we have reached bounds of yregion
            currenty = ydirection ? (currenty + 1) : (currenty - 1);
            if (currenty > gameRows - 1) {
                // turn around
                ydirection = false;
                currenty = currenty - 2;
            } else if (ydirection == false && currenty <= (gameRows - playerRows)) {
                ydirection = true;
            }
        }

        if (gameGrid[currentx][currenty] == 2 || gameGrid[currentx][currenty] == 1) {
            currentx = centipede[currentcentipede][0][x]; // keep the original x
            xdirection = !xdirection; // just switch x direction
            // move up or down in y
            currenty = ydirection ? (currenty + 1) : (currenty - 1);
            gameGrid[currentx][currenty] = 0;
        }

        centipede[currentcentipede][0][x] = currentx;
        centipede[currentcentipede][0][y] = currenty;
        isLeft[currentcentipede] = xdirection;
        isBottom[currentcentipede] = ydirection;
    }
}