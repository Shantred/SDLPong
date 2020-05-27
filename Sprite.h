#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "LTexture.h"

extern SDL_Renderer* gameRenderer;

class Sprite : public LTexture
{
public:
	// TODO: Move these private and use getters/setters. Refactor accordingly.
	int posX;
	int posY;

	Sprite(std::string path, int x, int y );
	~Sprite();
	void render();
	
	void setPosition(int x, int y);
};