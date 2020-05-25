#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

// For now, we will define a basic sprite as a texture that
// handles it's own rendering and movement
class Sprite
{
public:
	int posX;
	int posY;
	SDL_Texture* texture;

	Sprite(SDL_Renderer* renderer, std::string path, int x, int y );
	~Sprite();
	
	void setPosition(int x, int y);
	void render();
	
private:
	int width;
	int height;
	int angle;

	SDL_Renderer* renderer;
};