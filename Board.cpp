#include "Board.h"
#include "Piece.h"
Board::Tile Board::board[8][8];
bool Board::whitePlays;
Piece* Board::lastPieceMoved;
std::map<std::string, Piece> Board::pieces;
//loads and draws board;
void Board::LoadBoard() {
	sf::Color tan(255, 255, 204, 255);
	sf::Color grey(189, 113, 0, 255);
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
	whitePlays = true;
}
void Board::DrawBoard(sf::RenderWindow& window) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			window.draw(Board::board[i][j].tileGraphic);
		}
	}
	//Draw pieces
	/*auto iter = pieces.begin();
	for (iter; iter != pieces.end(); iter++) {
		iter->second.Draw(window);
	}*/
}
void Board::ResetBoard() {

}

Board::Tile* Board::TileClicked(sf::Vector2i& mouseClick) {
	Tile* tileClicked = nullptr;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			auto tileBounds = board[i][j].tileGraphic.getGlobalBounds();
			if (tileBounds.contains(mouseClick.x, mouseClick.y)) {
				tileClicked = &board[i][j];
			}
		}
	}
	return tileClicked;
}
void Board::Move(Piece& pieceClicked) {

}
void Board::UndoMove() {

}