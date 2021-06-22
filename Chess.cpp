#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.h"
#include "Piece.h"
#include "TextureManager.h"

using namespace std;
int main()
{
    //Set up board
    int width = 8 * 60;
    int height = 8 * 60 + 100;
    sf::RenderWindow window(sf::VideoMode(width, height), "Chess by Tristan and Jerami");
    Board::LoadBoard();

    //Draw pieces
    Piece testPiece(true, TextureManager::GetTexture("b-knight"), sf::Vector2f(120, 360));
    //Other variables
    bool movePiece = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {     //Check if piece was pressed
                sf::Vector2i mouseClick = sf::Mouse::getPosition(window);
                auto testPieceBounds = testPiece.GetImage().getGlobalBounds();
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (testPieceBounds.contains(mouseClick.x, mouseClick.y)) {
                        movePiece = true;
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    //highlight potential moves
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (movePiece) {
                    sf::Vector2i mouseLocation = sf::Mouse::getPosition(window);
                    testPiece.GetImage().setPosition(sf::Vector2f(mouseLocation));
                    Board::Tile* tileClicked = Board::TileClicked(mouseLocation);
                    testPiece.GetImage().setPosition(tileClicked->tileGraphic.getPosition());   //auto-position within a space
                    movePiece = false;
                }
                cout << "Mouse released" << endl;
            }
        }
        window.clear();
        Board::DrawBoard(window);
        window.draw(testPiece.GetImage());
        window.display();
        
    }
    TextureManager::Clear();
    return 0;
}