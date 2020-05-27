// Pong remake from scratch using SDL2
#include <stdio.h>
#include <SDL2/SDL.h>
#include "Game.h"
#include "LTexture.h"

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gameWindow = NULL;
SDL_Renderer* gameRenderer = NULL;
TTF_Font* gFont = NULL;

int main(int argc, char* args[])
{
	Game game;
	return 0;
}