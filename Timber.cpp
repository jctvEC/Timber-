// HelloSFML.cpp: Define o ponto de entrada para a aplicação de console.
//

#include "stdafx.h"
#include "Sprites.h"
#include <SFML/Graphics.hpp>
#define NUM_CLOUDS 3
using namespace sf;

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

	// Preparing the clouds.
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	moveableSprites cloudSprite[NUM_CLOUDS];
	for (int i = 0; i < NUM_CLOUDS; i++)
		cloudSprite[i] = moveableSprites(textureCloud, 0, i * 250);

	Clock clock;

	while (window.isOpen())	{
		
		/* 
		-----------------
		Handle user input.
		-----------------
		*/

		// O jogo fecha ao pressionar o Esc.
		if (Keyboard::isKeyPressed(Keyboard::Escape)) 
			window.close();

		/*
		-----------------
		Update the scene.
		-----------------
		*/

		Time dt = clock.restart();

		if (!beeSprite.isActive()) {
			// How fast the bee is.
			srand((int)time(0));
			beeSprite.setSpeed((rand() % 200) + 200);

			// How high the bee is.   
			srand((int)time(0) * 10);
			float height = (rand() % 500) + 500;
			beeSprite.setPosition(2000, height);
			beeSprite.setActiveness();
		} else {
			
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
	
		for (int i = 0; i < NUM_CLOUDS; i++)	{
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
				if (cloudSprite[i].getSprite().getPosition().x > 1920)	{
					// Set it up ready to be a whole new cloud next frame     
					cloudSprite[i].setActiveness();
				}
			}
		}


		/*
		-----------------
		Draw the current scene.
		-----------------
		*/
		window.clear();

		window.draw(backgroundSprite.getSprite());

		// Draw the clouds
		for (int i = 0; i < NUM_CLOUDS; i++)	{
			window.draw(cloudSprite[i].getSprite());
		}

		// Draw the tree
		window.draw(treeSprite.getSprite());

		// Draw the insect
		window.draw(beeSprite.getSprite());

		/*
		-----------------
		Display everything drew.
		-----------------
		*/
		window.display();
	}

    return 0;
}

