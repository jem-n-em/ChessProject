#ifndef PIECE
#define PIECE
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cmath>
//#include "Board.h"
class Piece;

struct Board
{
	struct Tile {
		bool isLight = false;
		sf::RectangleShape tileGraphic;
		short x;
		short y;
		Piece* pieceOnTile;
	};
	static Tile board[8][8];
	static bool whitePlays;

	//FUNCTIONS
	static void LoadBoard();
	static void DrawBoard(sf::RenderWindow& window);

	static Tile* TileClicked(sf::Vector2i& mouseClick);
};
class Piece
{
protected:
	bool white;
	bool captured;
	sf::Texture texture;
	sf::Sprite image;
	Board::Tile* initialPos;
	Board::Tile* currentPos;
	std::vector<Board::Tile*> potentialMoves;
public:
	Piece(bool white, sf::Texture texture, Board::Tile* pos);
	bool IsWhite();
	bool IsCaptured();
	void ToggleCapture();
	sf::Sprite& GetImage();
	Board::Tile* GetCurrentPos();
	void SetCurrentPos(Board::Tile* newPos);
	//virtual function: get list of possible spaces, place in vector
	std::vector<Board::Tile*>& GetPotentialMoves();	//returns potential moves vector without changes
	virtual std::vector<Board::Tile*>& GetMoves();	//returns updated list of potential moves
};
class King : public Piece
{
public:
	King(bool white, sf::Texture texture, Board::Tile* initialPos);
	std::vector<Board::Tile*>& GetMoves();
	//
};
class Pawn : public Piece
{
public:
	Pawn(bool white, sf::Texture texture, Board::Tile* initialPos);
	std::vector<Board::Tile*>& GetMoves();
};
class Knight : public Piece
{
public:
	Knight(bool white, sf::Texture texture, Board::Tile* initialPos);
	std::vector<Board::Tile*>& GetMoves();
};
class Rook : public Piece
{
public:
	Rook(bool white, sf::Texture texture, Board::Tile* initialPos);
	std::vector<Board::Tile*>& GetMoves();
};
class Bishop : public Piece
{
public:
	Bishop(bool white, sf::Texture texture, Board::Tile* initialPos);
	std::vector<Board::Tile*>& GetMoves();
};
class Queen : public Piece
{
public:
	Queen(bool white, sf::Texture texture, Board::Tile* initialPos);
	std::vector<Board::Tile*>& GetMoves();
};
#endif PIECE
