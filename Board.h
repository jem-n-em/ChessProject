#include <SFML/Graphics.hpp>
#include <map>
#include <iterator>
#include <iostream>
#include "Piece.h"
#include "TextureManager.h"
#pragma once
using std::iterator;

struct Board
{
	struct Tile {
		bool isOccupied = false;
		sf::RectangleShape tileGraphic;
		//sf::Piece* ; to determine if piece is occupied or not
	};
	static Tile board[8][8];
	static bool whitePlays;
	static Piece* lastPieceMoved;

	//Pieces
	static std::map<std::string, Piece> pieces;
	/*Piece whiteKing(true, TextureManager::GetTexture("w-king"), sf::Vector2f(240, 420));
	pieces.emplace("w-king", whiteKing);*/

	//FUNCTIONS
	static void LoadBoard();
	static void DrawBoard(sf::RenderWindow& window);
	static void ResetBoard();

	static Tile* TileClicked(sf::Vector2i& mouseClick);
	static void Move(Piece& pieceClicked);
	static void UndoMove();
};

