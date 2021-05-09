#include "TextureManager.h"
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName) {
	string fullName = "images/";
	fullName += fileName + ".png";
	textures[fileName].loadFromFile(fullName);
}
sf::Texture& TextureManager::GetTexture(string fileName) {
	if (textures.find(fileName) == textures.end())	//does not yet exist
		LoadTexture(fileName);

	return textures[fileName];
}
void TextureManager::Clear() {
	textures.clear();
}