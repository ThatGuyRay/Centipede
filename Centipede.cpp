#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
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
bool bulletCollision(float bullet[], int &score, int centipedearray[][maxCentipedeSize][2], int &numberofCentipede);
void pixelToGrid(float pixelarray[], int gridarray[]);
// void generateCentipede(int len, bool isleft, int headposition[], int& numberofCentipede, 
//     bool isLeft[], int centipedeLength[], int centipedeheadPositionX[], int centipedeheadPositionY[]);
void drawCentipede(sf::RenderWindow& window, sf::Texture& centipedeheadTexture, sf::Texture& centipedebodyTexture, 
    int maxnumber_centipede, int numberofcentipedes, int centipede[][maxCentipedeSize][2]);
void UpdateGrid(int numberofCentipede, int centipede[][maxCentipedeSize][2]);

void moveCentipede(int numberofCentipede, bool isLeft[], bool isBottom[], int centipede[][maxCentipedeSize][2]);
bool playerCentipedeCollision(int centipede[][maxCentipedeSize][2], int numberofCentipede, float player[]);
void generateCentipede(int centipedeLength, int headposition[], bool startingLeft, bool isLeft[], bool isBottom[],
    int centipede[][maxCentipedeSize][2], int& numberofCentipedes);

int main()
{
    srand(time(NULL));

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
    int maxnumber_centipede = 12;    bool isLeft[maxCentipedeSize] = {};
    bool isBottom[maxCentipedeSize] = {};
    int centipedearray[maxnumber_centipede][maxCentipedeSize][2] = {};

    //initialize centipede at a random row and between col 1 to 6
    int startingPosition[2] = {(rand() % gameRows), 1 + (rand() % 6)};
    int startingLeft = rand() % 2;
    generateCentipede(maxCentipedeSize, startingPosition, startingLeft, isLeft, isBottom, centipedearray, numberofCentipede);

    // Todo: Add Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);

    sf::SoundBuffer playerdeathbuffer;
    sf::Sound playerdeathsound;
    playerdeathbuffer.loadFromFile("Sound_Effects/death.wav");
    playerdeathsound.setBuffer(playerdeathbuffer);
    playerdeathsound.setVolume(50);

    sf::SoundBuffer mushroomkillbuffer;
    sf::Sound mushroomkillsound;
    mushroomkillbuffer.loadFromFile("Sound_Effects/kill.wav");
    mushroomkillsound.setBuffer(mushroomkillbuffer);
    mushroomkillsound.setVolume(50);

    sf::SoundBuffer bulletBuffer;
    sf::Sound bulletSound;
    bulletBuffer.loadFromFile("Sound_Effects/fire1.wav");
    bulletSound.setBuffer(bulletBuffer);
    bulletSound.setVolume(50);

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

    //Game Text
    sf::Font font;
    if(!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Font File not found" << std::endl;
    }
    //Score Text
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score = 0");
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    scoreText.setStyle(sf::Text::Bold);

    //Game over Text
    sf::Text gameoverText;
    gameoverText.setFont(font);
    gameoverText.setString("Game Over");
    gameoverText.setCharacterSize(60);
    gameoverText.setFillColor(sf::Color::Red);
    gameoverText.setPosition(300, 300);
    gameoverText.setStyle(sf::Text::Bold);

    //Rectangle for effect
    sf::RectangleShape rectangle(sf::Vector2f(960, 960));
    rectangle.setFillColor(sf::Color(128, 128, 128, 180));

    //Score
    int score = 0;
    int centipederate = 0;
    bool gameOver;
    bool bulletfired = false;

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

        // do nothing if game over
        if (gameOver) continue;

        window.clear(sf::Color::Black);
        window.draw(backgroundSprite);

        // draw everything here...
        movePlayer(player);
        drawPlayer(window, player, playerSprite);

        fireBullet(bullet, player);

        //Bullet creation and collision
        if (bullet[exists] == true) 
        {
            if(!bulletfired)
            {
                bulletSound.play();
                bulletfired = true;
            }
			moveBullet(bullet);
			drawBullet(window, bullet, bulletSprite);
            if(bulletCollision(bullet, score, centipedearray, numberofCentipede))
            {
                mushroomkillsound.play();
                bullet[exists] = false;
            }
        }

        if(!bullet[exists])
        {
            bulletfired = false;
        }
        //Movement of Centipede and game over text
        if(centipederate == 10)
        {
            moveCentipede(numberofCentipede, isLeft, isBottom, centipedearray);
            if(playerCentipedeCollision(centipedearray, numberofCentipede, player)) {
                // draw game over text
                window.draw(rectangle);
                window.draw(gameoverText);
                gameOver = true;
                playerdeathsound.play();
            }
            centipederate = 0;
        }
        centipederate++;

        UpdateGrid(numberofCentipede, centipedearray);    
        drawCentipede(window, centipedeheadTexture, centipedebodyTexture, maxnumber_centipede, numberofCentipede, centipedearray);
        

        drawMushrooms(window, mushroomTexture);

        std::stringstream ss;
        ss << "Score = " << score;
        scoreText.setString(ss.str());
        window.draw(scoreText);

        window.display();
    }
    return 0;
}


//Draws the player
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}

//Moves the player within bounds
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

//Draws the Bullet
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}

//Moves the Bullet
void moveBullet(float bullet[])
{
    bullet[y] -= 20;	
	if (bullet[y] < -32)
    {
		bullet[exists] = false;
    }
}

//Fires the Bullet
void fireBullet(float bullet[], float player[])
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) && !bullet[exists])
    {

        
        bullet[x] = player[x];
        bullet[y] = player[y] - boxPixelsY;
        bullet[exists] = true;
    }
}

//Draws the Mushrooms
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

//Function that turns the grid value to the Position value
void gridToPixel(int gridx, int gridy, int pixelarray[])
{
    pixelarray[0] = gridx * 32;
    pixelarray[1] = gridy * 32;
}

//Generates random mushrooms
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

//For Bullet Collision and Incrementing the Score
bool bulletCollision(float bullet[], int &score, int centipedearray[][maxCentipedeSize][2], int &numberofCentipede)
{
    int gridarray[2];
    int hitcounter = 0;
    pixelToGrid(bullet, gridarray);


    // detect centipede collision
    for (int currentcentipede = 0; currentcentipede < numberofCentipede; ++currentcentipede)
    {
        if (gridarray[0] == centipedearray[currentcentipede][0][0] && gridarray[1] == centipedearray[currentcentipede][0][1])
        {
            cout << "Hit the head" << endl;

            for (int k = currentcentipede; k < numberofCentipede; k++)
            {
                for (int i = 0; i < maxCentipedeSize - 1; ++i)
                {
                    for (int j = 0; j < maxCentipedeSize; ++j)
                    {
                        centipedearray[k][i][0] = centipedearray[k + 1][i + 1][0];
                        centipedearray[k][i][1] = centipedearray[k + 1][i + 1][1];
                    }
                }
            }
            return true;
        }
        for (int bodysegment = 0; bodysegment < maxCentipedeSize; bodysegment++)
        {

            if (gridarray[0] == centipedearray[currentcentipede][bodysegment][0] && gridarray[1] == centipedearray[currentcentipede][bodysegment][1])
            {

                // Calculate the length of the remaining segment
                int newCentipedeLenght = maxCentipedeSize - (bodysegment + 1);

                cout << "Created new Centipede with length " << newCentipedeLenght << endl;

                for (int newCentipedeSegments = 0; newCentipedeSegments < newCentipedeLenght; newCentipedeSegments++)
                {
                    // Change spawn point of the splitted centipede.
                    centipedearray[currentcentipede + 1][newCentipedeSegments][0] = centipedearray[currentcentipede][bodysegment + newCentipedeSegments][0] - 1;
                    centipedearray[currentcentipede + 1][newCentipedeSegments][1] = centipedearray[currentcentipede][bodysegment + newCentipedeSegments][1];

                    // Set segments of new centipede as inactive in the old centipede
                    centipedearray[currentcentipede][bodysegment + newCentipedeSegments][0] = -1;
                    centipedearray[currentcentipede][bodysegment + newCentipedeSegments][1] = -1;
                }

                numberofCentipede++;
                cout << "Hit centipede at " << gridarray[0] << " " << gridarray[1] << " " << numberofCentipede << endl;
                return true;
            }
        }
    }

    if (gameGrid[gridarray[0]][gridarray[1]] == 2 || gameGrid[gridarray[0]][gridarray[1]] == 1)
    {
        gameGrid[gridarray[0]][gridarray[1]]--;
        if (gameGrid[gridarray[0]][gridarray[1]] == 0)
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

//Converts the pixels to a grid value
void pixelToGrid(float pixelarray[], int gridarray[])
{
    gridarray[0] = pixelarray[0] / 31;
    gridarray[1] = pixelarray[1] / 32;
}

//Draws the Centipede 
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


//Putting the Centipede in the Grid
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

//Moving the Centipede
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
            gameGrid[currentx][currenty] = 0;
            currentx = centipede[currentcentipede][0][x]; // keep the original x
            xdirection = !xdirection; // just switch x direction
            // move up or down in y
            currenty = ydirection ? (currenty + 1) : (currenty - 1);
            
        }

        centipede[currentcentipede][0][x] = currentx;
        centipede[currentcentipede][0][y] = currenty;
        isLeft[currentcentipede] = xdirection;
        isBottom[currentcentipede] = ydirection;
    }
}

bool playerCentipedeCollision(int centipede[][maxCentipedeSize][2], int numberofCentipede, float player[]) {
    int position_of_player[2] = {};
    pixelToGrid(player, position_of_player);

    for (int currentcentipede = 0; currentcentipede < numberofCentipede; currentcentipede++) {
        for (int bodysegment = 0; bodysegment < maxCentipedeSize; bodysegment++) {
            // Check only if the segment exists
            if (centipede[currentcentipede][bodysegment][x] != -1) {
                int body_grid[2] = { centipede[currentcentipede][bodysegment][x], centipede[currentcentipede][bodysegment][y] };

                if (position_of_player[0] == body_grid[0] && position_of_player[1] == body_grid[1]) {
                    return true; // Collision detected
                }
            }
        }
    }
    return false; // No collision detected
}

void generateCentipede(int centipedeLength, int headposition[], bool startingLeft, bool isLeft[], bool isBottom[],
    int centipede[][maxCentipedeSize][2], int& numberofCentipedes)
{
    int centipedeIndex = numberofCentipedes;
    // Generating the head of the centipede
    centipede[centipedeIndex][0][x] = headposition[x];
    centipede[centipedeIndex][0][y] = headposition[y];

    // Generate body
    int currentX = headposition[x];
    int currentY = headposition[y];
    bool direction = startingLeft;
    for(int bodyIndex = 1; bodyIndex < maxCentipedeSize; bodyIndex++) {
        // centipede[centipedeIndex][bodyIndex][x] = currentX ? 
        
        if (bodyIndex < centipedeLength) {
            // increment X based on direction
            currentX += direction ? 1 : -1;
            // if X is out of screen update it and move Y up and switch direction
            if (currentX < 0) {
                currentX = 0;
                currentY--;
               direction = true;
            } else if (currentX > gameColumns - 1) {
                currentX = gameColumns - 1;
                currentY--;
                direction = false;
            }
            // assign X and Y
            centipede[centipedeIndex][bodyIndex][x] = currentX;
            centipede[centipedeIndex][bodyIndex][y] = currentY;
        } else {
            // mark body as complete by setting x, y = -1, -1 and break
            centipede[centipedeIndex][bodyIndex][x] = -1;
            centipede[centipedeIndex][bodyIndex][y] = -1;
            break;
         }
     }

    // set up other properties 
    isLeft[centipedeIndex] = startingLeft;
    isBottom[centipedeIndex] = true;
    numberofCentipedes++;
 }