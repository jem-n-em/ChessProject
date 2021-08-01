#ifndef BOARD
#define BOARD
#pragma once
#include <SFML/Graphics.hpp> 
#include "TextureManager.h"

struct Board
{
	struct Tile {
		bool isOccupied = false;
		bool isLight = false;
		sf::RectangleShape tileGraphic;
		short x;
		short y;
	};
	static Tile board[8][8];
	static bool whitePlays;
	
	//FUNCTIONS
	static void LoadBoard();
	static void DrawBoard(sf::RenderWindow& window);

	static Tile* TileClicked(sf::Vector2i& mouseClick);
};

#endif BOARD

