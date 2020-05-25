#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "Game.h"
#include "Sprite.h"
#include "Ball.h"
#include "Paddle.h"


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

/**
 * This class is a wrapper for all game functionality and serves as a manager for all game logic
 **/

Game::Game()
{
	bool paused = false;
	printf("Initializing game...\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	// Initialize sets up all subsystems and gives us a window and renderer
	if (Game::initWindow()) {

		// Once we have our window and renderer, draw our ball and paddles
		// Ball gets rendered at the center of the screen.
		Ball ball(Game::gameRenderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// DEBUG
		//Ball ball(Game::gameRenderer, WINDOW_WIDTH / 2, 30);

		// Create our player's paddle
		Paddle playerPaddle(Game::gameRenderer, 20, 280, 12);

		Paddle player2Paddle(Game::gameRenderer, 760, 280, 12);
		
		
		printf("Window should be present!\n");

		// Listen for quit message
		bool quit = false;
		while (!quit) {

			SDL_Event e;
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}
			SDL_SetRenderDrawColor(Game::gameRenderer, 0, 0, 0, 0xFF);
			SDL_RenderClear(Game::gameRenderer);

			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			if (currentKeyStates[SDL_SCANCODE_P]) {
				paused = true;
			}

			if (currentKeyStates[SDL_SCANCODE_O]) {
				paused = false;
			}

			if (!paused) {
				
				// Only move and render the paddle if they are pressing a directional key
				if (currentKeyStates[SDL_SCANCODE_UP]) {
					playerPaddle.move(Paddle::DIRECTION_UP);
				} else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
					playerPaddle.move(Paddle::DIRECTION_DOWN);
					
				}

				ball.move(playerPaddle.collider);

				// Check if the ball is out of bounds
				if (ball.getState() == Ball::BALL_OUT) {
					playerPaddle.resetPosition();
					// For the ball, we need to pass the paddle that won so we know which one to attach
					// the ball to.
					ball.setPosition(playerPaddle.collider.x, playerPaddle.collider.y);
					ball.setState(Ball::BALL_ON_PADDLE);
				}
				
				if (ball.getState() == Ball::BALL_ON_PADDLE) {
					if (currentKeyStates[SDL_SCANCODE_SPACE]) {
						// 1 here denotes positive x velocity. We would use -1 to go the other direction
						// We will handle this when we implement player 2
						ball.launch(1);
					}
				}

				
			}

			ball.render();
			playerPaddle.render();
			player2Paddle.render();
			
			SDL_RenderPresent(Game::gameRenderer);
		}
	} else {
		printf("Unable to initialize Game!\n");
	}
}

Game::~Game()
{
	SDL_DestroyRenderer(Game::gameRenderer);
	SDL_DestroyWindow(Game::gameWindow);
	Game::gameWindow = NULL;
	Game::gameRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

bool Game::initWindow()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Unable to initialize SDL! SDL Error: %s\n", SDL_GetError());
		return false;
	} else {
		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!\n");
		}

		// Create the game window
		Game::gameWindow = SDL_CreateWindow("SDL Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		if (Game::gameWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		} else {
			// Create renderer with vsync
			Game::gameRenderer = SDL_CreateRenderer(Game::gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (Game::gameRenderer == NULL) {
				printf("Unable to create renderer! SDL Error: %s\n", SDL_GetError());
				return false;
			} else {
				// Setup a default render color of black
				SDL_SetRenderDrawColor(Game::gameRenderer, 0, 0, 0, 0xFF);
				SDL_RenderClear(Game::gameRenderer);
			}
		}
	}
	
	return true;
}