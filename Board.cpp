#include "Board.h"
Board::Tile Board::board[8][8];
bool Board::whitePlays;
//loads and draws board;
void Board::LoadBoard() {
	sf::Color light(255, 255, 204, 255);
	sf::Color dark(189, 113, 0, 255);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			sf::RectangleShape& tile = Board::board[i][j].tileGraphic;
			tile.setSize(sf::Vector2f(60, 60));
			tile.setPosition(60 * i, 60 * j);
			Board::board[i][j].x = (short)i;
			Board::board[i][j].y = (short)j;
			if ((i + j) % 2 == 0) {
				tile.setFillColor(light);
				Board::board[i][j].isLight = true;
			}
			else {
				tile.setFillColor(dark);
			}		
		}
	}
	whitePlays = true;
}
void Board::DrawBoard(sf::RenderWindow& window) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			window.draw(Board::board[i][j].tileGraphic);
		}
	}
}
Board::Tile* Board::TileClicked(sf::Vector2i& mouseClick) {
	Tile* tileClicked = nullptr;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			auto tileBounds = board[i][j].tileGraphic.getGlobalBounds();	//use later for all piecesu
			if (tileBounds.contains(mouseClick.x, mouseClick.y)) {
				tileClicked = &board[i][j];
			}
		}
	}
	return tileClicked;
}