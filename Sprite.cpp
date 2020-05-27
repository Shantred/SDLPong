#include <SDL2/SDL.h>
#include "SDL_image.h"
#include <stdio.h>
#ifdef _WIN32
#include <string.h>
#else
#include <string>
#endif
#include "Sprite.h"
#include "Game.h"

// This class is a from-image texture that manages its' own position.
// Sprite should be extended by any object that needs to be moved frequently.
// For static objects, use LTexture instead.
Sprite::Sprite(std::string path, int x, int y)
{
	loadFromFile(path);
	Sprite::setPosition(x, y);
	render();
}

Sprite::~Sprite()
{
	free();
}

void Sprite::setPosition(int x, int y)
{
	Sprite::posX = x;
	Sprite::posY = y;

}

void Sprite::render() {
	LTexture::render(posX, posY);
}