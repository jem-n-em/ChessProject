#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <iterator>
#include <string>
#include <vector>
//#include "Board.h"
#include "Chess.h"
#include "TextureManager.h"
using namespace std;

Piece* PieceClicked(sf::Vector2i mouseClick, map<string, Piece*>& pieces);
void DrawPieces(map<string, Piece*>& pieces, sf::RenderWindow& window);
void HighlightSpaces(Piece* piece, bool highlight);
void Move(Piece& piece);
int main()
{
    //Set up board
    int width = 8 * 60;
    int height = 8 * 60 + 100;
    sf::RenderWindow window(sf::VideoMode(width, height), "Chess by Tristan and Jerami");
    Board::LoadBoard();

    /*========SET UP PIECES========*/
    map<string, Piece*> pieces;
    //white pieces
    pieces.emplace("w-king", new King(true, TextureManager::GetTexture("w-king"), &Board::board[3][7]));
    pieces.emplace("w-queen", new Queen(true, TextureManager::GetTexture("w-queen"), &Board::board[4][7]));
    pieces.emplace("w-rook_1", new Rook(true, TextureManager::GetTexture("w-rook"), &Board::board[0][7]));
    pieces.emplace("w-rook_2", new Rook(true, TextureManager::GetTexture("w-rook"), &Board::board[7][7]));
    pieces.emplace("w-knight_1", new Knight(true, TextureManager::GetTexture("w-knight"), &Board::board[1][7]));
    pieces.emplace("w-knight_2", new Knight(true, TextureManager::GetTexture("w-knight"), &Board::board[6][7]));
    pieces.emplace("w-bishop_1", new Bishop(true, TextureManager::GetTexture("w-bishop"), &Board::board[2][7]));
    pieces.emplace("w-bishop_2", new Bishop(true, TextureManager::GetTexture("w-bishop"), &Board::board[5][7]));
    pieces.emplace("w-pawn_1", new Pawn(true, TextureManager::GetTexture("w-pawn"), &Board::board[0][6]));
    pieces.emplace("w-pawn_2", new Pawn(true, TextureManager::GetTexture("w-pawn"), &Board::board[1][6]));
    pieces.emplace("w-pawn_3", new Pawn(true, TextureManager::GetTexture("w-pawn"), &Board::board[2][6]));
    pieces.emplace("w-pawn_4", new Pawn(true, TextureManager::GetTexture("w-pawn"), &Board::board[3][6]));
    pieces.emplace("w-pawn_5", new Pawn(true, TextureManager::GetTexture("w-pawn"), &Board::board[4][6]));
    pieces.emplace("w-pawn_6", new Pawn(true, TextureManager::GetTexture("w-pawn"), &Board::board[5][6]));
    pieces.emplace("w-pawn_7", new Pawn(true, TextureManager::GetTexture("w-pawn"), &Board::board[6][6]));
    pieces.emplace("w-pawn_8", new Pawn(true, TextureManager::GetTexture("w-pawn"), &Board::board[7][6]));

    //black pieces
    pieces.emplace("b-king", new King(false, TextureManager::GetTexture("b-king"), &Board::board[3][0]));
    pieces.emplace("b-queen", new Queen(false, TextureManager::GetTexture("b-queen"), &Board::board[4][0]));
    pieces.emplace("b-rook_1", new Rook(false, TextureManager::GetTexture("b-rook"), &Board::board[0][0]));
    pieces.emplace("b-rook_2", new Rook(false, TextureManager::GetTexture("b-rook"), &Board::board[7][0]));
    pieces.emplace("b-knight_1", new Knight(false, TextureManager::GetTexture("b-knight"), &Board::board[1][0]));
    pieces.emplace("b-knight_2", new Knight(false, TextureManager::GetTexture("b-knight"), &Board::board[6][0]));
    pieces.emplace("b-bishop_1", new Bishop(false, TextureManager::GetTexture("b-bishop"), &Board::board[2][0]));
    pieces.emplace("b-bishop_2", new Bishop(false, TextureManager::GetTexture("b-bishop"), &Board::board[5][0]));
    pieces.emplace("b-pawn_1", new Pawn(false, TextureManager::GetTexture("b-pawn"), &Board::board[0][1]));
    pieces.emplace("b-pawn_2", new Pawn(false, TextureManager::GetTexture("b-pawn"), &Board::board[1][1]));
    pieces.emplace("b-pawn_3", new Pawn(false, TextureManager::GetTexture("b-pawn"), &Board::board[2][1]));
    pieces.emplace("b-pawn_4", new Pawn(false, TextureManager::GetTexture("b-pawn"), &Board::board[3][1]));
    pieces.emplace("b-pawn_5", new Pawn(false, TextureManager::GetTexture("b-pawn"), &Board::board[4][1]));
    pieces.emplace("b-pawn_6", new Pawn(false, TextureManager::GetTexture("b-pawn"), &Board::board[5][1]));
    pieces.emplace("b-pawn_7", new Pawn(false, TextureManager::GetTexture("b-pawn"), &Board::board[6][1]));
    pieces.emplace("b-pawn_8", new Pawn(false, TextureManager::GetTexture("b-pawn"), &Board::board[7][1]));
    
    /*========OTHER VARIABLES========*/
    Piece* pieceClicked = nullptr;
    bool movePiece = false;
    bool highlight = false;
    bool whiteMoves = true;
    //Piece* lastPieceMoved;
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {     //Check if piece was pressed
                sf::Vector2i mouseClick = sf::Mouse::getPosition(window);
                pieceClicked = PieceClicked(mouseClick, pieces);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (pieceClicked != nullptr && !highlight)                        //If a piece was actually clicked on
                        movePiece = true;
                }
                if (event.mouseButton.button == sf::Mouse::Right) {     //highlight potential moves
                    if (pieceClicked != nullptr) {
                        if (!highlight) {   //turn highlight on
                            highlight = true;
                            cout << "Piece moves highlighted" << endl;
                            HighlightSpaces(pieceClicked, highlight);
                        }
                        else {              //turn highlight off
                            highlight = false;
                            cout << "Piece moves unhighlighted" << endl;
                            HighlightSpaces(pieceClicked, highlight);
                            pieceClicked = nullptr;
                        }    
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (movePiece) {        //Piece movement
                    sf::Vector2i mouseLocation = sf::Mouse::getPosition(window);
                    Board::Tile* tileClicked = Board::TileClicked(mouseLocation);
                    pieceClicked->SetCurrentPos(tileClicked);   //auto-position within a space
                    pieceClicked = nullptr;
                    movePiece = false;
                }
            }
        }
        /*=========DRAW PHASE==========*/
        window.clear();
        Board::DrawBoard(window);
        DrawPieces(pieces, window);
        window.display();
        
    }
    TextureManager::Clear();
    return 0;
}

/*========HELPING METHODS========*/
Piece* PieceClicked(sf::Vector2i mouseClick, map<string, Piece*>& pieces) {
    auto iter = pieces.begin();
    for (iter = pieces.begin(); iter != pieces.end(); iter++) {
        if (iter->second->GetImage().getGlobalBounds().contains(mouseClick.x, mouseClick.y)) {
            return iter->second;
        }
    }
    if (iter == pieces.end()) {
        return nullptr;
    }
}
void DrawPieces(map<string, Piece*>& pieces, sf::RenderWindow& window) {
    auto iter = pieces.begin();
    for (iter = pieces.begin(); iter != pieces.end(); iter++) {
        if(!iter->second->IsCaptured())
            window.draw(iter->second->GetImage());
    }
}
void HighlightSpaces(Piece* piece, bool highlight) {
    sf::Color lightYellow(255, 255, 102, 255);
    sf::Color darkYellow(255, 217, 25, 255);
    sf::Color vanilla(255, 255, 205, 255);
    sf::Color dark(189, 113, 0, 255);
    if (highlight) {        //highlight mode is on
        for (unsigned int i = 0; i < piece->GetMoves().size(); i++) {
            Board::Tile* tile = piece->GetMoves()[i];
            if (tile->isLight)
                tile->tileGraphic.setFillColor(lightYellow);
            else
                tile->tileGraphic.setFillColor(darkYellow);
        }
    }
    else {      //convert back to regular colors
        for (unsigned int i = 0; i < piece->GetMoves().size(); i++) {
            Board::Tile* tile = piece->GetMoves()[i];
            if (tile->isLight)
                tile->tileGraphic.setFillColor(vanilla);
            else
                tile->tileGraphic.setFillColor(dark);
        }
    }

}