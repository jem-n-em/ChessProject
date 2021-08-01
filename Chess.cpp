#include "Chess.h"
/*========BOARD STRUCT FUNCTIONS========*/
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

/*========PIECE CLASS FUNCTIONS========*/

Piece::Piece(bool white, sf::Texture texture, Board::Tile* initialPos) {
	this->white = white;
	this->texture = texture;
	this->initialPos = initialPos;
	currentPos = initialPos;
	currentPos->pieceOnTile = this;
	image.setTexture(this->texture);
	image.setPosition(initialPos->tileGraphic.getPosition());
	captured = false;
}
bool Piece::IsWhite() {
	return white;
}
bool Piece::IsCaptured() {
	return captured;
}
void Piece::ToggleCapture() {
	captured = !captured;
}
sf::Sprite& Piece::GetImage() {
	return image;
}
Board::Tile* Piece::GetCurrentPos() {
	return currentPos;
}
void Piece::SetCurrentPos(Board::Tile* newPos) {
	currentPos->pieceOnTile = nullptr;
	currentPos = newPos;
	currentPos->pieceOnTile = this;
	image.setPosition(currentPos->tileGraphic.getPosition());
}
std::vector<Board::Tile*>& Piece::GetPotentialMoves() {
	return potentialMoves;
}
std::vector<Board::Tile*>& Piece::GetMoves() {	
	potentialMoves.clear();
	potentialMoves.push_back(&Board::board[0][7]);
	return potentialMoves;
}
/*===King Functions===*/
King::King(bool white, sf::Texture texture, Board::Tile* initialPos)
	: Piece{white, texture, initialPos}
{}
std::vector<Board::Tile*>& King::GetMoves() {
	potentialMoves.clear();
	bool adjacent;
	bool allyOccupied;
	for (int i = 0; i < 8; i++) {			//loop through all squares; find tiles adjacent to King
		for (int j = 0; j < 8; j++) {
			Board::Tile* tile = &Board::board[i][j];
			adjacent = std::abs(currentPos->x - tile->x) <= 1 && std::abs(currentPos->y - tile->y) <= 1;
			if (i == currentPos->x && j == currentPos->y) {	//skip current tile
				continue;
			}
			if (adjacent) {
				//check if allied pieces occupy an adjacent space; cannot move King there
				allyOccupied = (tile->pieceOnTile != nullptr) && (tile->pieceOnTile->IsWhite() == this->white);
				if (!allyOccupied)
					potentialMoves.push_back(tile);
			}
		}
	}
	//Edge cases: pins/checks, same-color occupied, dif-color occupied
	return potentialMoves;
}
/*===Pawn Functions===*/
Pawn::Pawn(bool white, sf::Texture texture, Board::Tile* initialPos)
	: Piece{white, texture, initialPos}
{}
std::vector<Board::Tile*>& Pawn::GetMoves() {
	potentialMoves.clear();
	Board::Tile* diagonal;
	Board::Tile* oneForward;
	Board::Tile* twoForward;
	if (white) {	/*WHITE PAWNS*/
		if (currentPos->y != 0) {
			oneForward = &Board::board[currentPos->x][currentPos->y - 1];
			if (oneForward->pieceOnTile == nullptr) {
				potentialMoves.push_back(&Board::board[currentPos->x][currentPos->y - 1]);
			}
		}
		if (currentPos == initialPos) {	//first move can be two spaces
			oneForward = &Board::board[currentPos->x][currentPos->y - 1];
			twoForward = &Board::board[currentPos->x][currentPos->y - 2];
			if (twoForward->pieceOnTile == nullptr && oneForward->pieceOnTile == nullptr) {
				potentialMoves.push_back(&Board::board[currentPos->x][currentPos->y - 2]);
			}
		}
		//diagonal capturing
		if (currentPos->x != 0) {
			diagonal = &Board::board[currentPos->x - 1][currentPos->y - 1];
			if (diagonal->pieceOnTile != nullptr && diagonal->pieceOnTile->IsWhite() != this->white) {
				potentialMoves.push_back(diagonal);
			}
		}
		if (currentPos->x != 7) {
			diagonal = &Board::board[currentPos->x + 1][currentPos->y - 1];
			if (diagonal->pieceOnTile != nullptr && diagonal->pieceOnTile->IsWhite() != this->white) {
				potentialMoves.push_back(diagonal);
			}
		}
	}
	else {	/*BLACK PAWNS*/
		if (currentPos->y != 7) {
			oneForward = &Board::board[currentPos->x][currentPos->y + 1];
			if (oneForward->pieceOnTile == nullptr) {
				potentialMoves.push_back(&Board::board[currentPos->x][currentPos->y + 1]);
			}
		}
		if (currentPos == initialPos) {	//first move can be two spaces
			oneForward = &Board::board[currentPos->x][currentPos->y + 1];
			twoForward = &Board::board[currentPos->x][currentPos->y + 2];
			if (twoForward->pieceOnTile == nullptr && oneForward->pieceOnTile == nullptr) {
				potentialMoves.push_back(&Board::board[currentPos->x][currentPos->y + 2]);
			}
		}
		//diagonal capturing
		if (currentPos->x != 0) {
			diagonal = &Board::board[currentPos->x - 1][currentPos->y + 1];
			if (diagonal->pieceOnTile != nullptr && diagonal->pieceOnTile->IsWhite() != this->white) {
				potentialMoves.push_back(diagonal);
			}
		}
		if (currentPos->x != 7) {
			diagonal = &Board::board[currentPos->x + 1][currentPos->y + 1];
			if (diagonal->pieceOnTile != nullptr && diagonal->pieceOnTile->IsWhite() != this->white) {
				potentialMoves.push_back(diagonal);
			}
		}
	}

	//en passant???
	return potentialMoves;
}
/*===Knight Functions===*/
Knight::Knight(bool white, sf::Texture texture, Board::Tile* initialPos)
	: Piece{ white, texture, initialPos }
{}
std::vector<Board::Tile*>& Knight::GetMoves() {
	potentialMoves.clear();
	bool allyOccupied;
	//sum of distances strategy: xDistance and yDistance must add up to three, be between 1 and 2;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Board::Tile* tile = &Board::board[i][j];
			int xDist = std::abs(currentPos->x - i);
			int yDist = std::abs(currentPos->y - j);
			if (xDist + yDist == 3 && xDist <= 2 && yDist <= 2) {
				allyOccupied = tile->pieceOnTile != nullptr && tile->pieceOnTile->IsWhite() == this->white;
				if (!allyOccupied) {
					potentialMoves.push_back(tile);
				}
			}
		}
	}
	return potentialMoves;
}
/*===Rook===*/
Rook::Rook(bool white, sf::Texture texture, Board::Tile* initialPos)
	: Piece{ white, texture, initialPos }
{}
std::vector<Board::Tile*>& Rook::GetMoves() {
	potentialMoves.clear();
	int x = currentPos->x;
	int y = currentPos->y;
	Board::Tile* tile;
	//up
	while (y > 0) {
		y--;
		tile = &Board::board[currentPos->x][y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white)
				break;
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	//bring x and y placeholders back to piece's position
	x = currentPos->x;
	y = currentPos->y;		
	//down
	while (y < 7) {
		y++;
		tile = &Board::board[currentPos->x][y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white)
				break;
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	x = currentPos->x;
	y = currentPos->y;
	//left
	while (x > 0) {
		x--;
		tile = &Board::board[x][currentPos->y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white)
				break;
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	x = currentPos->x;
	y = currentPos->y;
	//right
	while (x < 7) {
		x++;
		tile = &Board::board[x][currentPos->y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white)
				break;
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	return potentialMoves;
}
/*===Bishop===*/
Bishop::Bishop(bool white, sf::Texture texture, Board::Tile* initialPos)
	: Piece{ white, texture, initialPos }
{}
std::vector<Board::Tile*>& Bishop::GetMoves() {
	potentialMoves.clear();
	int x = currentPos->x;
	int y = currentPos->y;
	Board::Tile* tile;
	//left up
	while (x > 0 && y > 0) {
		x--;
		y--;
		tile = &Board::board[x][y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white) {
				break;
			}
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	//bring x and y placeholders back to piece's position
	x = currentPos->x;
	y = currentPos->y;
	//right up
	while (x < 7 && y > 0) {
		x++;
		y--;
		tile = &Board::board[x][y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white) {
				break;
			}
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	x = currentPos->x;
	y = currentPos->y;
	//left down
	while (x > 0 && y < 7) {
		x--;
		y++;
		tile = &Board::board[x][y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white) {
				break;
			}
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	x = currentPos->x;
	y = currentPos->y;
	//right down
	while (x < 7 && y < 7) {
		x++;
		y++;
		tile = &Board::board[x][y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white) {
				break;
			}
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	x = currentPos->x;
	y = currentPos->y;
	return potentialMoves;
}
/*===Queen===*/
Queen::Queen(bool white, sf::Texture texture, Board::Tile* initialPos)
	: Piece{ white, texture, initialPos }
{}
std::vector<Board::Tile*>& Queen::GetMoves() {
	potentialMoves.clear();
	int x = currentPos->x;
	int y = currentPos->y;
	Board::Tile* tile;

	/*===HORIZONTAL/VERTICAL===*/
	//up
	while (y > 0) {
		y--;
		tile = &Board::board[currentPos->x][y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white)
				break;
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	//bring x and y placeholders back to piece's position
	x = currentPos->x;
	y = currentPos->y;
	//down
	while (y < 7) {
		y++;
		tile = &Board::board[currentPos->x][y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white)
				break;
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	x = currentPos->x;
	y = currentPos->y;
	//left
	while (x > 0) {
		x--;
		tile = &Board::board[x][currentPos->y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white)
				break;
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	x = currentPos->x;
	y = currentPos->y;
	//right
	while (x < 7) {
		x++;
		tile = &Board::board[x][currentPos->y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white)
				break;
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	x = currentPos->x;
	y = currentPos->y;
	
	/*===DIAGONALS===*/
	//left up
	while (x > 0 && y > 0) {
		x--;
		y--;
		tile = &Board::board[x][y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white) {
				break;
			}
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	x = currentPos->x;
	y = currentPos->y;
	//right up
	while (x < 7 && y > 0) {
		x++;
		y--;
		tile = &Board::board[x][y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white) {
				break;
			}
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	x = currentPos->x;
	y = currentPos->y;
	//left down
	while (x > 0 && y < 7) {
		x--;
		y++;
		tile = &Board::board[x][y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white) {
				break;
			}
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	x = currentPos->x;
	y = currentPos->y;
	//right down
	while (x < 7 && y < 7) {
		x++;
		y++;
		tile = &Board::board[x][y];
		if (tile->pieceOnTile != nullptr) {
			if (tile->pieceOnTile->IsWhite() == this->white) {
				break;
			}
			else {
				potentialMoves.push_back(tile);
				break;
			}
		}
		potentialMoves.push_back(tile);
	}
	x = currentPos->x;
	y = currentPos->y;
	return potentialMoves;
}