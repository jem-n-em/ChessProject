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
/*========HELPER FUNCTIONS========*/
Piece* PieceClicked(sf::Vector2i mouseClick, map<string, Piece*>& pieces);
void LoadPieces(map<string, Piece*>& pieces);
void LoadCapturedPieces(vector<Piece*>& capturedPieces, sf::RenderWindow& window);
void DrawPieces(map<string, Piece*>& pieces, vector<Piece*>& capturedPieces, sf::RenderWindow& window);
void HighlightSpaces(Piece* piece, bool highlight);
bool Move(Piece* piece, Board::Tile* tileClicked);
void Take(Piece* piece, vector<Piece*>& capturedPieces);
void ResetBoard(map<string, Piece*>& pieces, vector<Piece*>& capturedPieces);
int main()
{
    /*=========CONSTRUCT BOARD/WINDOW========*/
    const int WIDTH = 8;
    const int HEIGHT = 8;
    int windowWidth = WIDTH * 60;
    int windowHeight = HEIGHT * 60 + 100;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Chess by Tristan and Jerami");
    Board::LoadBoard();

    /*========SET UP PIECES========*/
    map<string, Piece*> pieces;
    LoadPieces(pieces);
    vector<Piece*> capturedPieces;
    /*========OTHER VARIABLES========*/
    Piece* pieceClicked = nullptr;
    bool movePiece = false;
    bool highlight = false;
    bool whiteMoves = true;
    //Piece* lastPieceMoved;
    //Graveyard setup
    sf::Color lightBrown(180, 142, 100, 255);
    sf::Color darkBrown(102, 82, 52, 255);
    sf::RectangleShape graveyard;
    float graveyardLen = 30 * 7.5f;
    float graveyardWid = 60.0f;
    graveyard.setSize(sf::Vector2f(graveyardLen, graveyardWid));
    graveyard.setPosition(0, WIDTH * 60);
    graveyard.setFillColor(lightBrown);
    graveyard.setOutlineColor(darkBrown);
    graveyard.setOutlineThickness(-5.0f);
    //Reset Button Setup
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::CircleShape reset;
    reset.setRadius(28.0f);
    reset.setFillColor(lightBrown);
    reset.setOutlineColor(darkBrown);
    reset.setOutlineThickness(5.0f);
    reset.setPosition(7 * 60, HEIGHT * 60);
    sf::Text resetText("RESET", font, 15);
    resetText.setStyle(sf::Text::Bold);
    resetText.setFillColor(sf::Color::Black);
    resetText.setPosition(7 * 60 + 5, HEIGHT * 60 + 20);
    auto resetButton = reset.getGlobalBounds();

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
                    if (pieceClicked != nullptr && !pieceClicked->IsCaptured() && !highlight) //Checks if a valid piece was clicked
                        movePiece = true;
                    else if (resetButton.contains(mouseClick.x, mouseClick.y)) {
                        ResetBoard(pieces, capturedPieces);
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right) { //highlight moves
                    if (pieceClicked != nullptr && !pieceClicked->IsCaptured())
                    {
                        highlight = true;
                        HighlightSpaces(pieceClicked, highlight);
                    } 
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left && movePiece) { //piece movement
                    sf::Vector2i mouseLocation = sf::Mouse::getPosition(window);
                    Board::Tile* tileClicked = Board::TileClicked(mouseLocation);
                    if (Move(pieceClicked, tileClicked)) {
                        if (tileClicked->pieceOnTile != nullptr) {
                            Take(tileClicked->pieceOnTile, capturedPieces);
                            LoadCapturedPieces(capturedPieces, window);
                        }
                        pieceClicked->SetCurrentPos(tileClicked);
                    }
                    pieceClicked = nullptr;
                    movePiece = false;
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {  //Unhighlight moves
                    if (pieceClicked != nullptr && !pieceClicked->IsCaptured()) {
                        highlight = false;
                        HighlightSpaces(pieceClicked, highlight);
                        pieceClicked = nullptr;
                    } 
                }
            }
        }
        /*=========DRAW COMPONENTS==========*/
        window.clear();
        Board::DrawBoard(window);
        window.draw(graveyard);
        window.draw(reset);
        window.draw(resetText);
        DrawPieces(pieces, capturedPieces, window);
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
void LoadPieces(map<string, Piece*>& pieces) {
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
}
void LoadCapturedPieces(vector<Piece*>& capturedPieces, sf::RenderWindow& window) {
    int whiteX = 0;
    int whiteY = 60 * 8;
    int blackX = 0;
    int blackY = 60 * 8 + 30; 
    for (unsigned int i = 0; i < capturedPieces.size(); i++) {
        capturedPieces.at(i)->GetImage().setScale(0.5f, 0.5f);
        if (capturedPieces.at(i)->IsWhite()) {
            capturedPieces.at(i)->GetImage().setPosition(whiteX, whiteY);
            whiteX += 15;
        }
        else {
            capturedPieces.at(i)->GetImage().setPosition(blackX, blackY);
            blackX += 15;
        }
    }
}
void DrawPieces(map<string, Piece*>& pieces, vector<Piece*>& capturedPieces, sf::RenderWindow& window) {
    auto iter = pieces.begin();
    for (iter = pieces.begin(); iter != pieces.end(); iter++) {
        if(!iter->second->IsCaptured())
            window.draw(iter->second->GetImage());
    }
    for (unsigned int i = 0; i < capturedPieces.size(); i++) {  //draw captured pieces
        window.draw(capturedPieces.at(i)->GetImage());
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
void Take(Piece* piece, vector<Piece*>& capturedPieces) {
    piece->ToggleCapture();
    capturedPieces.push_back(piece);
}
bool Move(Piece* piece, Board::Tile* tileClicked) {
    for (unsigned int i = 0; i < piece->GetMoves().size(); i++) {
        if (piece->GetMoves()[i] == tileClicked) {
            //piece->SetCurrentPos(tileClicked);
            return true;
        }       
    }
    return false;
}
void ResetBoard(map<string, Piece*>& pieces, vector<Piece*>& capturedPieces) {
    capturedPieces.clear();
    auto iter = pieces.begin();
    for (iter; iter != pieces.end(); iter++) {
        iter->second->ResetPos();
    }
}