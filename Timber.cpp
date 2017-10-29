/*	Timber!!! Game.
	Developed by João Victor de Sá Ferraz Coutinho.
	My record is 278. */

#include "stdafx.h"
#include "Sprites.h"
#include "Texts.h"
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
using namespace std;

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Posição do jogador/ramo.
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

void updateBranches(int seed);
side setRandomPlayerSide();
float setRandomTreePosition(int seed);

int main() {

	// Preparando o Vídeo.
	VideoMode vm(1366, 768);
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	View view(FloatRect(0, 0, 1920, 1080));
	window.setView(view);

	/*****************************************************/

	// Preparando texturas e sprites.

	// Background.
	Texture backgroundTexture;
	backgroundTexture.loadFromFile("graphics/background.png");
	Sprites backgroundSprite(backgroundTexture, 0, 0);

	// Árvore principal.
	Texture treeTexture;
	treeTexture.loadFromFile("graphics/tree.png");
	Sprites treeSprite(treeTexture, 810, 0);

	// Árvores de fundo.
	const int NUM_TREES = 2;
	Texture treeTexture2;
	treeTexture2.loadFromFile("graphics/tree2.png");
	Sprites treeSprite2[NUM_TREES];
	
	// Definindo as posições.
	treeSprite2[1] = Sprites(treeTexture2, setRandomTreePosition(1), 0);
	treeSprite2[0] = Sprites(treeTexture2, setRandomTreePosition(2), -150);
	
	// Abelha.
	Texture beeTexture;
	beeTexture.loadFromFile("graphics/bee.png");
	moveableSprites beeSprite(beeTexture, 0, 800);

	// Nuvens.
	const int NUM_CLOUDS = 3;

	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	moveableSprites cloudSprite[NUM_CLOUDS];
	for (int i = 0; i < NUM_CLOUDS; i++)
		cloudSprite[i] = moveableSprites(textureCloud, 0, i * 250);

	// Jogador.
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprites spritePlayer(texturePlayer, 580, 720);
	// O jogador começa à esquerda.
	side playerSide = setRandomPlayerSide();
	if (playerSide == side::RIGHT)
		spritePlayer.setPosition(1200, 720);

	// Tumba.
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprites spriteRIP(textureRIP, 600, 2000);

	// Machado.
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprites spriteAxe(textureAxe, 700, 830);

	// Alinha o machado com a árvore principal.
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;
	if (playerSide == side::RIGHT)
		spriteAxe.setPosition(AXE_POSITION_RIGHT, 830);

	// Toro.
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	moveableSprites spriteLog(textureLog, 810, 720);
	spriteLog.setSpeed(-1000, -1500);

	// Galhos.
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	Sprites branches[NUM_BRANCHES];
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i] = Sprites(textureBranch, -2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	/*****************************************************/

	// Preparando sons.

	// Cortando.
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	// Morte.
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// Tempo esgotado. 
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	/*****************************************************/

	// HUD.

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	Texts messageText(font, Color::White, 75, 1920 / 2.0f, 1080 / 2.0f);
	messageText.setString("Press Enter to start!");

	// Posicionando o texto.
	FloatRect textRect = messageText.getText().getLocalBounds();
	messageText.setOrigin(
		textRect.left +	textRect.width / 2.0f,
		textRect.top + 	textRect.height / 2.0f);

	// Colocando um fundo.
	RectangleShape rect1;
	rect1.setFillColor(Color(0, 0, 0, 150));
	rect1.setSize(Vector2f(600, 105));
	rect1.setPosition(0, 30);

	Texts scoreText(font, Color::White, 100, 20, 20);

	/*****************************************************/

	// Controle de tempo.
	
	// Barra de tempo.
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	//Time gameTimeTotal; 
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	/*****************************************************/

	// Pause.
	bool paused = true;
	bool gameOver = false;

	// Score.
	int score = 0;

	// Controle da entrada.
	bool acceptInput = false;

	// Tempo.
	Clock clock;

	while (window.isOpen())	{
		
		/* Handle user input. */

		Event event;
		while (window.pollEvent(event))	{
			if (event.type == Event::KeyReleased && !paused) {

				// Aceite entradas.
				acceptInput = true;

				// Esconda o machado.
				spriteAxe.setPosition(2000,
					spriteAxe.getSprite().getPosition().y);
			}
		}

		// O jogo fecha ao pressionar o Esc.
		if (Keyboard::isKeyPressed(Keyboard::Escape)) 
			window.close();

		// O jogo reinicia ao pressionar o Enter.
		if (Keyboard::isKeyPressed(Keyboard::Return)) {

			paused = false;
			gameOver = false;

			// Reseta o tempo e o score.
			score = 0;
			timeRemaining = 6.0f;

			// Todos os ramos desaparecem. 
			for (int i = 1; i < NUM_BRANCHES; i++)
				branchPositions[i] = side::NONE;

			// A tumba desaparece.
			spriteRIP.setPosition(675, 2000);

			// Move o jogador para a posição.
			if (playerSide == side::LEFT)
				spritePlayer.setPosition(580, 720);
			else
				spritePlayer.setPosition(1200, 720);

			acceptInput = true;
		}

		if (acceptInput) {

			// Lidar com a tecla ->
			if (Keyboard::isKeyPressed(Keyboard::Right)) {

				// O jogador é movido para a direita.
				playerSide = side::RIGHT;

				score++;

				// Adiciona ao tempo restante.
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getSprite().getPosition().y);

				spritePlayer.setPosition(1200, 720);

				// Atualiza os ramos.
				updateBranches(score);

				// O toro voa para a esquerda.
				spriteLog.setPosition(810, 720);
				spriteLog.setSpeed(-5000, spriteLog.getSpeedY());
				spriteLog.setActiveness(true);

				acceptInput = false;

				// Toca o som de corte.
				chop.play();
			}

			// Lidar com a tecla <-
			if (Keyboard::isKeyPressed(Keyboard::Left))	{

				// O jogador é movido para a esquerda.
				playerSide = side::LEFT;

				score++;

				// Adiciona ao tempo restante. 
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getSprite().getPosition().y);

				spritePlayer.setPosition(580, 720);

				// Atualiza os ramos.
				updateBranches(score);

				// O toro voa para a direita.
				spriteLog.setPosition(810, 720);
				spriteLog.setSpeed(5000, spriteLog.getSpeedY());
				spriteLog.setActiveness(true);

				acceptInput = false;

				// Toca o som de corte. 
				chop.play();
			}
			
		}

		/* Update the scene. */
		Time dt = clock.restart();
		if (!paused) {

			// Subtrai do tempo restante. 
			timeRemaining -= dt.asSeconds();
			// Atualiza a barra de tempo.
			timeBar.setSize(Vector2f(timeBarWidthPerSecond *
				timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f) {

				// Pausa o jogo.  
				paused = true;
				gameOver = true;

				// Mostra uma mensagem ao jogador e o reposiciona.
				messageText.setString("Tempo esgotado!!");

				FloatRect textRect = messageText.getText().getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Toca o som de timeout. 
				outOfTime.play();
			}
			
			stringstream ss;
			ss << "Score: " << score;
			scoreText.setString(ss.str());

			// update the branch sprites   
			for (int i = 0; i < NUM_BRANCHES; i++) {

				float height = i * 150;
				if (branchPositions[i] == side::LEFT) {

					// Move the sprite to the left side        
					branches[i].setPosition(610, height);

					// Flip the sprite round the other way        
					branches[i].setRotation(180);
				} 
				
				else if (branchPositions[i] == side::RIGHT) {
					// Move the sprite to the right side        
					branches[i].setPosition(1330, height);

					// Set the sprite rotation to normal        
					branches[i].setRotation(0);
				}

				else {
					// Hide the branch        
					branches[i].setPosition(3000, height);
				}

			}

			// Handle a flying log.       
			if (spriteLog.isActive()) {

				spriteLog.setPosition(
					spriteLog.getSprite().getPosition().x +
					(spriteLog.getSpeedX() * dt.asSeconds()),

					spriteLog.getSprite().getPosition().y +
					(spriteLog.getSpeedY() * dt.asSeconds()));

				// Has the log reached the right hand edge? 
				if (spriteLog.getSprite().getPosition().x < -100 ||
					spriteLog.getSprite().getPosition().x > 2000) {

					// Set it up ready to be a whole new log next frame 
					spriteLog.setActiveness(false);
					spriteLog.setPosition(810, 720);
				}
			}

			// Has the player been squished by a branch? 
			if (branchPositions[5] == playerSide) {
				// death 
				paused = true;
				gameOver = true;
				acceptInput = false;

				// Draw the gravestone.
				if (playerSide == side::LEFT)
					spriteRIP.setPosition(525, 760);
				else
					spriteRIP.setPosition(1230, 760);

				// hide the player 
				spritePlayer.setPosition(2000, 660);

				// Change the text of the message 
				messageText.setString("ESMAGADO!!");

				// Center it on the screen 
				FloatRect textRect = messageText.getText().getLocalBounds();

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f,
					1080 / 2.0f);

				// Play the death sound 
				death.play();
			}
		}

		if (!gameOver) {
			if (!beeSprite.isActive()) {
				// How fast the bee is.
				srand((int)time(0));
				beeSprite.setSpeed((rand() % 200) + 200, 0);

				// How high the bee is.   
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				beeSprite.setPosition(2000, height);
				beeSprite.setActiveness(true);
			}
			else {

				// Move the bee
				beeSprite.setPosition(
					beeSprite.getSprite().getPosition().x -
					(beeSprite.getSpeedX() * dt.asSeconds()),
					beeSprite.getSprite().getPosition().y);

				// Has the bee reached the right hand edge of the screen?   
				if (beeSprite.getSprite().getPosition().x < -100) {
					// Set it up ready to be a whole new bee next frame     
					beeSprite.setActiveness(false);
				}
			}

			for (int i = 0; i < NUM_CLOUDS; i++) {
				if (!cloudSprite[i].isActive()) {
					srand((int)time(0) * (i + 1) * 10);
					cloudSprite[i].setSpeed(rand() % 200, 0);

					srand((int)time(0) * 10);
					float height = (rand() % ((i + 1) * 150));
					if (i > 0)
						height -= 150;
					cloudSprite[i].setPosition(-200, height);
					cloudSprite[i].setActiveness(true);
				}
				else {
					cloudSprite[i].setPosition(
						cloudSprite[i].getSprite().getPosition().x +
						(cloudSprite[i].getSpeedX() * dt.asSeconds()),
						cloudSprite[i].getSprite().getPosition().y);

					// Has the cloud reached the right hand edge of the screen?   
					if (cloudSprite[i].getSprite().getPosition().x > 1920) {
						// Set it up ready to be a whole new cloud next frame     
						cloudSprite[i].setActiveness(false);
					}
				}
			}
		}

		/*	Draw the current scene */

		window.clear();

		window.draw(backgroundSprite.getSprite());

		// Draw the clouds
		for (int i = 0; i < NUM_CLOUDS; i++) {
			window.draw(cloudSprite[i].getSprite());
		}

		for (int i = 0; i < NUM_TREES; i++) {
			window.draw(treeSprite2[i].getSprite());
		}

		// Draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i].getSprite());
		}

		// Draw the trees
		window.draw(treeSprite.getSprite());
	
		// Draw the player 
		window.draw(spritePlayer.getSprite());

		// Draw the axe 
		window.draw(spriteAxe.getSprite());

		// Draraw the flying log 
		window.draw(spriteLog.getSprite());

		// Draw the gravestone 
		window.draw(spriteRIP.getSprite());

		// Draw the insect
		window.draw(beeSprite.getSprite());

		// Draw the timebar
		window.draw(timeBar);

		// Draw the score
		window.draw(scoreText.getText());
		if (paused)
			// Draw our message   
			window.draw(messageText.getText());


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

side setRandomPlayerSide() {
	srand((int)time(0));
	int side = (rand() % 2);

	if (side == 0)
		return side::LEFT;
	else
		return side::RIGHT;
}

float setRandomTreePosition(int seed) {
	srand((int)time(0) * seed * 10);
	float positionx = (rand() % 1920);
	if (positionx >= 710 && positionx <= 910)
		positionx += 300;
	return positionx;
}