#pragma once
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL2/SDL_ttf.h>
#else
#include "SDL_ttf.h"
#endif
#include "Ball.h"
#include "Paddle.h"
#include "LTexture.h"

extern SDL_Window* gameWindow;
extern SDL_Renderer* gameRenderer;
extern TTF_Font* gFont;

class Game
{
public:
	Game();
	~Game();
	
	enum GameState {
		GAME_IN_PROGRESS,
		GAME_PAUSED,
		GAME_OVER,
		TOTAL_GAME_STATES
	};

	GameState state = Game::GAME_IN_PROGRESS;
	int p1Score = 0;
	int p2Score = 0;
	LTexture p1ScoreText;
	LTexture p2ScoreText;
	LTexture winningText;

	

private:
	bool initWindow();
	bool loadMedia();
	bool checkCollisions(Ball& ball, Paddle& p1Paddle, Paddle& p2Paddle);
	void updateScores();
	void displayWinText(Uint8 player); // Should be 1 or 2 for player number
};