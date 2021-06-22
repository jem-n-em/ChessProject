#pragma once
#include <SFML/Graphics.hpp>
class Piece
{
	bool captured;
	bool white;
	sf::Texture texture;
	sf::Sprite image;
	sf::Vector2f initialPos;
public:
	Piece(bool white, sf::Texture texture, sf::Vector2f pos);
	bool IsCaptured();
	bool IsWhite();
	void ToggleCapture();
	void Draw(sf::RenderWindow& window);
	sf::Sprite& GetImage();
	//virtual function: get list of possible spaces, place in array
	//move is a standard function for all pieces; utilizes array
	virtual void Move();
};

