#include <SFML/Graphics.hpp>
#include <string>
#pragma once

//forward declaration later; write piece classes externally

struct Board
{
	struct Tile {
		bool isOccupied = false;
		sf::RectangleShape tileGraphic;
		//string tileName;
	};
	static Tile board[8][8];
	
	//FUNCTIONS
	static void LoadBoard();
	static void DrawBoard(sf::RenderWindow& window);
	static void ResetBoard();
	static void UndoMove();
};

