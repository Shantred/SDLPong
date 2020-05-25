#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "Sprite.h"


Sprite::Sprite(SDL_Renderer* renderer, std::string path, int x, int y)
{
	Sprite::renderer = renderer;
	
	// Load the sprite into texture
	Sprite::texture = NULL;
	SDL_Surface* tempSurface = NULL;
	tempSurface = IMG_Load(path.c_str());

	if (tempSurface == NULL) {
		printf("Error loading sprite image %s SDL Error: %s\n", path.c_str(), IMG_GetError());
	}
	Sprite::texture = SDL_CreateTextureFromSurface(Sprite::renderer, tempSurface);

	if (Sprite::texture == NULL) {
		printf("Error creating texture from %s SDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	// Get sprite dimensions
	Sprite::width = tempSurface->w;
	Sprite::height = tempSurface->h;

	// Clean up after our temp variables
	SDL_FreeSurface(tempSurface);
	tempSurface = NULL;

	Sprite::setPosition(x, y);
	Sprite::render();
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(Sprite::texture);
	Sprite::texture = NULL;
}

void Sprite::setPosition(int x, int y)
{
	Sprite::posX = x;
	Sprite::posY = y;

}

void Sprite::render()
{
	SDL_Rect renderQuad = { Sprite::posX, Sprite::posY, Sprite::width, Sprite::height };
	SDL_RenderCopy(Sprite::renderer, Sprite::texture, NULL, &renderQuad);
}
