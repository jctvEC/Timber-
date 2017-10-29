#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;

class Texts {

private:
	Text text;

public:
	Texts(const Font &font, const Color &color, int size, float positionx, float positiony) {
		this->text.setFont(font);
		this->text.setFillColor(color);
		this->text.setCharacterSize(size);
		this->text.setPosition(positionx, positiony);
	};

	Text getText() {
		return this->text;
	};

	void setString(String s) {
		this->text.setString(s);
	};

	void setOrigin(float originx, float originy) {
		this->text.setOrigin(originx, originy);
	}

	void setPosition(float positionx, float positiony) {
		this->text.setPosition(positionx, positiony);
	}
};