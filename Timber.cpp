// HelloSFML.cpp: Define o ponto de entrada para a aplicação de console.
//

#include "stdafx.h"
#include "Sprites.h"
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
using namespace std;

// Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Where is the player/branch?
// Left or Right
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];


int main() {

	// Creating Video Mode and RenderWindow.
	VideoMode vm(1366, 768);
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	View view(FloatRect(0, 0, 1920, 1080));
	window.setView(view);

	// Setting the background.
	Texture backgroundTexture;
	backgroundTexture.loadFromFile("graphics/background.png");
	Sprites backgroundSprite(backgroundTexture, 0, 0);

	// Setting the tree.
	Texture treeTexture;
	treeTexture.loadFromFile("graphics/tree.png");
	Sprites treeSprite(treeTexture, 810, 0);

	// Preparing the bee.
	Texture beeTexture;
	beeTexture.loadFromFile("graphics/bee.png");
	moveableSprites beeSprite(beeTexture, 0, 800);

	const int NUM_CLOUDS = 3;

	// Preparing the clouds.
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	moveableSprites cloudSprite[NUM_CLOUDS];
	for (int i = 0; i < NUM_CLOUDS; i++)
		cloudSprite[i] = moveableSprites(textureCloud, 0, i * 250);

	// HUD.
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	Text messageText;
	Text scoreText;

	messageText.setFont(font);
	scoreText.setFont(font);

	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = ");

	// Position the text
	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);

	// Time control.
	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Pause system.
	bool paused = true;

	// Score system.
	int score = 0;

	// Prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		// Set the sprite's origin to dead center   
		// We can then spin it round without changing its position 
		branches[i].setOrigin(220, 20);
	}

	// Prepare the player 
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	// The player starts on the left 
	side playerSide = side::LEFT;

	// Prepare the gravestone 
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// Prepare the axe 
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	// Line the axe up with the tree 
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Prepare the flying log 
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	// Some other useful log related variables 
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control the player input 
	bool acceptInput = false;

	// Prepare the sound 
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// Out of time 
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);


	while (window.isOpen())	{
		
		/* Handle user input. */

		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				// Listen for key presses again 
				acceptInput = true;

				// hide the axe 
				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}
		}

		// O jogo fecha ao pressionar o Esc.
		if (Keyboard::isKeyPressed(Keyboard::Escape)) 
			window.close();

		// O jogo pausa ao pressionar o Enter.
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;

			// Reset the time and the score   
			score = 0;
			timeRemaining = 6;

			// Make all the branches disappear 
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			// Make sure the gravestone is hidden 
			spriteRIP.setPosition(675, 2000);

			// Move the player into position 
			spritePlayer.setPosition(580, 720);

			acceptInput = true;
		}

		if (acceptInput)
		{
			// More code here next... 

			// First handle pressing the right cursor key 
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				// Make sure the player is on the right 
				playerSide = side::RIGHT;

				score++;

				// Add to the amount of time remaining 
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				// update the branches 
				updateBranches(score);

				// set the log flying to the left 
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
				// Play a chop sound 
				chop.play();
			}

			// Handle the left cursor key 

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				// Make sure the player is on the left 
				playerSide = side::LEFT;

				score++;

				// Add to the amount of time remaining 
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				// update the branches 
				updateBranches(score);

				// set the log flying 
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				// Play a chop sound 
				chop.play();
			}
			
		}


		/*	Update the scene.	*/

		if (!paused) {
			Time dt = clock.restart();

			// Subtract from the amount of time remaining   
			timeRemaining -= dt.asSeconds();
			// size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond *
				timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f) {
				// Pause the game   
				paused = true;

				// Change the message shown to the player   
				messageText.setString("Out of time!!");

				//Reposition the text based on its new size   
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Play the out of time sound 
				outOfTime.play();
			}


			if (!beeSprite.isActive()) {
				// How fast the bee is.
				srand((int)time(0));
				beeSprite.setSpeed((rand() % 200) + 200);

				// How high the bee is.   
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				beeSprite.setPosition(2000, height);
				beeSprite.setActiveness();
			}
			else {

				// Move the bee
				beeSprite.setPosition(
					beeSprite.getSprite().getPosition().x -
					(beeSprite.getSpeed() * dt.asSeconds()),
					beeSprite.getSprite().getPosition().y);

				// Has the bee reached the right hand edge of the screen?   
				if (beeSprite.getSprite().getPosition().x < -100) {
					// Set it up ready to be a whole new bee next frame     
					beeSprite.setActiveness();
				}
			}

			for (int i = 0; i < NUM_CLOUDS; i++) {
				if (!cloudSprite[i].isActive()) {
					srand((int)time(0) * (i + 1) * 10);
					cloudSprite[i].setSpeed(rand() % 200);

					srand((int)time(0) * 10);
					float height = (rand() % ((i + 1) * 150));
					if (i > 0)
						height -= 150;
					cloudSprite[i].setPosition(-200, height);
					cloudSprite[i].setActiveness();
				}
				else {
					cloudSprite[i].setPosition(
						cloudSprite[i].getSprite().getPosition().x +
						(cloudSprite[i].getSpeed() * dt.asSeconds()),
						cloudSprite[i].getSprite().getPosition().y);

					// Has the cloud reached the right hand edge of the screen?   
					if (cloudSprite[i].getSprite().getPosition().x > 1920) {
						// Set it up ready to be a whole new cloud next frame     
						cloudSprite[i].setActiveness();
					}
				}
			}

			stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// update the branch sprites   
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprite to the left side        
					branches[i].setPosition(610, height);

					// Flip the sprite round the other way        
					branches[i].setRotation(180);
				}

				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side        
					branches[i].setPosition(1330, height);

					// Set the sprite rotation to normal        
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branch        
					branches[i].setPosition(3000, height);
				}
			}

			// Handle a flying log         
			if (logActive)
			{

				spriteLog.setPosition(
					spriteLog.getPosition().x +
					(logSpeedX * dt.asSeconds()),

					spriteLog.getPosition().y +
					(logSpeedY * dt.asSeconds()));

				// Has the log reached the right hand edge? 
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					// Set it up ready to be a whole new log next frame 
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// Has the player been squished by a branch? 
			if (branchPositions[5] == playerSide)
			{
				// death 
				paused = true;
				acceptInput = false;

				// Draw the gravestone 
				spriteRIP.setPosition(525, 760);

				// hide the player 
				spritePlayer.setPosition(2000, 660);

				// Change the text of the message 
				messageText.setString("SQUISHED!!");

				// Center it on the screen 
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f,
					1080 / 2.0f);

				// Play the death sound 
				death.play();


			}


		}

		/*	Draw the current scene */

		window.clear();

		window.draw(backgroundSprite.getSprite());

		// Draw the clouds
		for (int i = 0; i < NUM_CLOUDS; i++)	{
			window.draw(cloudSprite[i].getSprite());
		}

		// Draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}


		// Draw the tree
		window.draw(treeSprite.getSprite());

		// Draw the player 
		window.draw(spritePlayer);

		// Draw the axe 
		window.draw(spriteAxe);

		// Draraw the flying log 
		window.draw(spriteLog);

		// Draw the gravestone 
		window.draw(spriteRIP);


		// Draw the insect
		window.draw(beeSprite.getSprite());

		// Draw the timebar
		window.draw(timeBar);

		// Draw the score
		window.draw(scoreText);
		if (paused)
			// Draw our message   
			window.draw(messageText);


		/* Display everything drew.	*/
		window.display();
	}

    return 0;
}

// Function definition 
void updateBranches(int seed)
{
	// Move all the branches down one place 
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0   
	// LEFT, RIGHT or NONE   
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;

	case 1:
		branchPositions[0] = side::RIGHT;
		break;

	default:
		branchPositions[0] = side::NONE;
		break;
	}

}

