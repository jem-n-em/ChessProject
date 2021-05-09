#include "Board.h"
Board::Tile Board::board[8][8];
//loads and draws board;
void Board::LoadBoard() {
	sf::Color tan(219, 204, 188, 255);
	sf::Color grey(54, 54, 54, 255);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			sf::RectangleShape& tile = Board::board[i][j].tileGraphic;
			tile.setSize(sf::Vector2f(60, 60));
			tile.setPosition(60 * i, 60 * j);
			if ((i + j) % 2 == 0)
				tile.setFillColor(tan);
			else
				tile.setFillColor(grey);	
		}
	}
}
void Board::DrawBoard(sf::RenderWindow& window) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			window.draw(Board::board[i][j].tileGraphic);
		}
	}
}