#include <SFML/Graphics.hpp>
#include "Board.h"
#include "TextureManager.h"

using namespace std;
int main()
{
    int width = 8 * 60;
    int height = 8 * 60 + 100;
    sf::RenderWindow window(sf::VideoMode(width, height), "Chess by Tristan and Jerami");
    //Draw/Load Board
    Board::LoadBoard();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        Board::DrawBoard(window);
        window.display();
    }

    return 0;
}