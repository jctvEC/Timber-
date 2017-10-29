#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;

class Sprites {

protected:
	Sprite sprite;

public:
	Sprites() {

	};

	Sprites(Texture &texture, int positionx, int positiony) {
		sprite.setTexture(texture);
		sprite.setPosition(positionx, positiony);
	};

	void setPosition(float positionx, float positiony) {
		sprite.setPosition(positionx, positiony);
	};

	void setOrigin(float originx, float originy) {
		sprite.setOrigin(originx, originy);
	};

	void setRotation(float angle) {
		sprite.setRotation(angle);
	};

	Sprite getSprite() {
		return sprite;
	};

};

class moveableSprites : public Sprites {

private:
	float speed[2]; // Speed(x, y);
	bool active;

public:	

	moveableSprites() : Sprites() {

	};

	moveableSprites(Texture &texture, int positionx, int positiony) : Sprites(texture, positionx, positiony) {
		sprite.setTexture(texture);
		sprite.setPosition(positionx, positiony);
		speed[0] = 0.0f;
		speed[1] = 0.0f;
		active = false;
	};

	bool isActive() {
		return active;
	};

	float getSpeedX() {
		return speed[0];
	};

	float getSpeedY() {
		return speed[1];
	};

	void setSpeed(float speedX, float speedY) {
		this->speed[0] = speedX;
		this->speed[1] = speedY;
	};

	void setActiveness(bool active) {
		this->active = active;
	};
	
};