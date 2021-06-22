#include "Piece.h"
Piece::Piece(bool white, sf::Texture texture, sf::Vector2f pos) {
	this->white = white;
	this->texture = texture;
	image.setTexture(this->texture);
	image.setPosition(pos);
	captured = false;
}
bool Piece::IsCaptured() {
	return captured;
}
bool Piece::IsWhite() {
	return white;
}
void Piece::ToggleCapture() {
	captured = !captured;
}
void Piece::Draw(sf::RenderWindow& window) {
	window.draw(image);
}
sf::Sprite& Piece::GetImage() {
	return image;
}
void Piece::Move() {

}