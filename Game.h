#pragma once
#include <SDL2/SDL.h>
class Game
{
public:
	Game();
	~Game();
	
	enum GameState {
		GAME_NEW,
		GAME_IN_PROGRESS,
		GAME_PAUSED,
		GAME_OVER,
		TOTAL_GAME_STATES
	};

	GameState state;
	int playerScore;
	int AIScore;

	

private:
	bool initWindow();
	SDL_Window* gameWindow = NULL;
	SDL_Renderer* gameRenderer = NULL;
};