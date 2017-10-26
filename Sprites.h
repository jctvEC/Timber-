#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;

class Sprites {

public:
	Sprite sprite;

	Sprites() {

	};

	Sprites(Texture &texture, int positionx, int positiony) {
		sprite.setTexture(texture);
		sprite.setPosition(positionx, positiony);
	};

	Sprite getSprite() {
		return sprite;
	};

};

class moveableSprites : public Sprites {

private:
	float speed;
	bool active;

public:	

	moveableSprites() : Sprites() {

	};

	moveableSprites(Texture &texture, int positionx, int positiony) : Sprites(texture, positionx, positiony) {
		sprite.setTexture(texture);
		sprite.setPosition(positionx, positiony);
		speed = 0.0f;
		active = false;
	};

	bool isActive() {
		return active;
	};

	int getSpeed() {
		return speed;
	};

	void setSpeed(int speed) {
		this->speed = speed;
	};

	void setActiveness() {
		this->active = !this->active;
	};

	void setPosition(float positionx, float positiony) {
		sprite.setPosition(positionx, positiony);
	};

	
	
};