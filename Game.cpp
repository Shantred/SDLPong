#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL2/SDL_image.h>
#else
#include "SDL_image.h"
#endif
#ifdef __APPLE__
#include <SDL2/SDL_ttf.h>
#else
#include "SDL_ttf.h"
#endif
#include <stdio.h>
#include <string>
#include "Game.h"
#include "LTexture.h"
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
	printf("Initializing game...\n");
	// Initialize sets up all subsystems and gives us pointers to the window and renderer
	if (Game::initWindow()) {

		// Just loads our font texture for now. Could be used later to cache all textures into the GPU.
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		}

		// Once we have our window and renderer, draw our background, ball, paddles, scores
		// Ball rendered to center by default, should be moved to P1 paddle once game starts.
		Ball ball(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// Single image for the "net". 
		Sprite background("./assets/image/separator.png", 385, 5);

		// Create our player paddles
		Paddle playerPaddle(20, 280, 12);
		Paddle player2Paddle(760, 280, 12);

		// Call update to load our default score textures
		updateScores();
		
		// Listen for quit message
		bool quit = false;

		// Set a paddle to track when the game first starts, default to P1
		Paddle *trackPaddle = &playerPaddle;

		// Set the ball state to being on the paddle
		ball.setState(Ball::BALL_ON_PADDLE);

		while (!quit) {

			SDL_Event e;
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}

			SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 0xFF);
			SDL_RenderClear(gameRenderer);

			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			if (currentKeyStates[SDL_SCANCODE_P]) {
				Game::state = Game::GAME_PAUSED;
			}

			if (currentKeyStates[SDL_SCANCODE_O]) {
				Game::state = Game::GAME_IN_PROGRESS;
			}

			if (Game::state == Game::GAME_IN_PROGRESS) {
				
				// P2 Controls
				if (currentKeyStates[SDL_SCANCODE_UP]) {
					player2Paddle.move(Paddle::DIRECTION_UP);
				} else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
					player2Paddle.move(Paddle::DIRECTION_DOWN);
					
				}

				// P1 Controls
				if (currentKeyStates[SDL_SCANCODE_W]) {
					playerPaddle.move(Paddle::DIRECTION_UP);
				} else if (currentKeyStates[SDL_SCANCODE_S]) {
					playerPaddle.move(Paddle::DIRECTION_DOWN);
					
				}
				
				// Check collisions before moving. This prevents the ball from continuing to move past the paddles.
				Game::checkCollisions(ball, playerPaddle, player2Paddle);

				// We move the ball AFTER the paddle intentionally to give the players a slight collision advantage.
				ball.move(trackPaddle);

				// Check if the ball is out of bounds
				if (ball.getState() == Ball::BALL_OUT_RIGHT || ball.getState() == Ball::BALL_OUT_LEFT) {
					playerPaddle.resetPosition();
					player2Paddle.resetPosition();

					// P1 scored
					if (ball.getState() == Ball::BALL_OUT_RIGHT) {
						p1Score += 1;
						ball.setPosition(playerPaddle.collider.x, playerPaddle.collider.y);
						ball.velX = -1;
						trackPaddle = &playerPaddle;
					}

					// P2 Scored
					if (ball.getState() == Ball::BALL_OUT_LEFT) {
						p2Score += 1;
						ball.setPosition(player2Paddle.collider.x, player2Paddle.collider.y);
						ball.velX = 1;
						trackPaddle = &player2Paddle;
					}

					// Only call updateScores when the ball first goes out because we want to limit the amount of 
					// texture loads we do.
					updateScores();

					// Check for win condition: First player to 6
					if (p1Score >= 6) {
						Game::state = Game::GAME_OVER;
						displayWinText(1);
					}

					if (p2Score >= 6) {
						Game::state = Game::GAME_OVER;
						displayWinText(2);
					}
				
					ball.setState(Ball::BALL_ON_PADDLE);
				}
				
				// Launch ball when space is pressed, but only when on paddle.
				if (ball.getState() == Ball::BALL_ON_PADDLE) {
					if (currentKeyStates[SDL_SCANCODE_SPACE]) {
						// Ball launches based on current velocity, which is reset when a player scores.
						ball.launch();
					}
				}

				
			}
			
			// Allow the game to be restarted if the current game has ended
			if (Game::state == Game::GAME_OVER) {
				if (currentKeyStates[SDL_SCANCODE_R]) {
					playerPaddle.resetPosition();
					player2Paddle.resetPosition();
					p1Score = 0;
					p2Score = 0;
					updateScores();
					Game::state = Game::GAME_IN_PROGRESS;
				}
			}

			// Draw calls
		
			// Render only what is needed based on game state
			if (Game::state == Game::GAME_IN_PROGRESS) {
				// Background must render first so the ball goes over it
				background.render();
				playerPaddle.render();
				player2Paddle.render();

				// Likewise, Ball goes last so it renders on top of everything else. 
				ball.render();
			}
			
			// Score text is always rendered
			p1ScoreText.render(200, 30);
			p2ScoreText.render(600, 30);
			
			if (Game::state == Game::GAME_OVER) {
				winningText.render(80, 300);
			}
			
			SDL_RenderPresent(gameRenderer);
		}
	} else {
		printf("Unable to initialize Game!\n");
	}
}

Game::~Game()
{
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);
	gameWindow = NULL;
	gameRenderer = NULL;

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
		gameWindow = SDL_CreateWindow("SDL Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		if (gameWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		} else {
			// Create renderer with vsync
			gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gameRenderer == NULL) {
				printf("Unable to create renderer! SDL Error: %s\n", SDL_GetError());
				return false;
			} else {
				// Setup a default render color of black
				SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 0xFF);
				//SDL_RenderClear(gameRenderer);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					return false;
				}

				if (TTF_Init() == -1) {
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					return false;
				}
			}
		}
	}
	
	return true;
}

bool Game::loadMedia()
{
	bool success = true;

	gFont = TTF_OpenFont("./assets/font/Arimo-Bold.ttf", 42);
	if (gFont == NULL) {
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	return success;
}

bool Game::checkCollisions(Ball& ball, Paddle& p1Paddle, Paddle& p2Paddle) {
	// The ball has limited colliding states with the paddle. It may collide on the right(p1), left(p2), top or bottom faces of the paddle.
	// Note that this math is relative to the direction the ball is going.
	// First, we determine if the ball is within the paddle at all. To do this, we treat the paddle as a box and check
	// to see if either corner of the ball(since the ball is actually a square) is within it.
	// If the ball is within the paddle, its' x <= the x + width of the paddle and the y of the ball is between the paddle's y or y + paddle height.
	// We also need to check both left corners of the paddle on the Y axis to check for partial collision(such as what would happen
	// if the ball collided with the top or bottom edge of the paddle.

	printf("Ball Information: \n Sprite X, Y: <%i, %i>\n Collider X, Y: <%i, %i> \n Velocity X, Y: <%i, %i>\n isColliding: %i\n", ball.posX, ball.posY, ball.collider.x, ball.collider.y, ball.velX, ball.velY, ball.isColliding);
	printf("P1 Paddle Information: \n Collider X, Y: <%i, %i>\n", p1Paddle.collider.x, p1Paddle.collider.y);
	printf("P2 Paddle Information: \n Collider X, Y: <%i, %i>\n", p2Paddle.collider.x, p2Paddle.collider.y);

	// Note: The below comment was written while considering the left(p1) player. 
	// Our collision checks are fairly straight-forward. We will check if the ball is at or beyond the paddle on the X axis.
	// Then check if the Y coordinate of the ball is BETWEEN the top and bottom of the paddle. We do this with a check at both
	// the top and the bottom of the ball collider so that we can detect edge collisions.
	// In order for collision to be true, one or both of those Y axis checks need to be true.
	// If both Y conditions are true, we have a head-on collision. If only one of them is true, we have a top or bottom collision.
	
	// Check for collisions for P1
	bool collideXP1 = (ball.collider.x < (p1Paddle.collider.x + p1Paddle.collider.w));
	bool collideYTopP1 = ((ball.collider.y + ball.BALL_HEIGHT) > p1Paddle.collider.y && (ball.collider.y + ball.BALL_HEIGHT) < (p1Paddle.collider.y + p1Paddle.collider.h));
	bool collideYBotP1 = (ball.collider.y > p1Paddle.collider.y && ball.collider.y < (p1Paddle.collider.y + p1Paddle.collider.h));
	if (collideXP1) {
		printf("collideX P1: %i, collideYTop: %i, collideYBot: %i\n", collideXP1, collideYTopP1, collideYBotP1);
	}

	// Check collisions for p2
	bool collideXP2 = ((ball.collider.x + ball.BALL_WIDTH) > p2Paddle.collider.x);
	bool collideYTopP2 = ((ball.collider.y + ball.BALL_HEIGHT) > p2Paddle.collider.y && (ball.collider.y + ball.BALL_HEIGHT) < (p2Paddle.collider.y + p2Paddle.collider.h));
	bool collideYBotP2 = (ball.collider.y > p2Paddle.collider.y && ball.collider.y < (p2Paddle.collider.y + p2Paddle.collider.h));
	if (collideXP2) {
		printf("collideX P2: %i, collideYTop: %i, collideYBot: %i\n", collideXP2, collideYTopP2, collideYBotP2);
	}

	// On the frame the collision first occurs, we set isColliding to true. As long as that is true, we do not apply
	// any additional collision effects and will wait for collideX to be false to set it back to false.
	if ((!collideXP1 && !collideXP2) && ball.isColliding) {
		printf("Ball no longer colliding \n");
		ball.isColliding = false;
	}

	if (!ball.isColliding && collideXP1 && (collideYTopP1 || collideYBotP1)) {
		ball.isColliding = true;

		// To deflect the ball at different angles depending on where it lands, we need to determine where we landed on the paddle
		// These velocity values were found by tinkering with what felt/looked right.
		float distanceFromPaddleCenterP1 = (p1Paddle.collider.y + (p1Paddle.collider.h / 2)) - (ball.collider.y + (ball.BALL_HEIGHT / 2));
		
		if (distanceFromPaddleCenterP1 == 0) {
			ball.velY = 5;
		} else if (distanceFromPaddleCenterP1 > 0 && distanceFromPaddleCenterP1 <= 15) {
			ball.velY = -7;
		} else if (distanceFromPaddleCenterP1 > 15) {
			ball.velY = -9;
		} else if (distanceFromPaddleCenterP1 < 0 && distanceFromPaddleCenterP1 >= -15) {
			ball.velY = 7;
		} else if (distanceFromPaddleCenterP1 < -15) {
			ball.velY = 9;
		}

		ball.velX *= -1;

		printf("Ball Y velocity: %i\n", ball.velY);
		printf("distance from paddle center P1 %f\n", distanceFromPaddleCenterP1);
		
		return true;
	}

	if (!ball.isColliding && collideXP2 && (collideYTopP2 || collideYBotP2)) {
		ball.isColliding = true;

		// To deflect the ball at different angles depending on where it lands, we need to determine where we landed on the paddle
		// These velocity values were found by tinkering with what felt/looked right.
		float distanceFromPaddleCenterP2 = (p2Paddle.collider.y + (p2Paddle.collider.h / 2)) - (ball.collider.y + (ball.BALL_HEIGHT / 2));
		
		if (distanceFromPaddleCenterP2 == 0) {
			ball.velY = 5;
		} else if (distanceFromPaddleCenterP2 > 0 && distanceFromPaddleCenterP2 <= 15) {
			ball.velY = -7;
		} else if (distanceFromPaddleCenterP2 > 15) {
			ball.velY = -9;
		} else if (distanceFromPaddleCenterP2 < 0 && distanceFromPaddleCenterP2 >= -15) {
			ball.velY = 7;
		} else if (distanceFromPaddleCenterP2 < -15) {
			ball.velY = 9;
		}

		ball.velX *= -1;

		printf("Ball Y velocity: %i\n", ball.velY);
		printf("distance from paddle center P2 %f\n", distanceFromPaddleCenterP2);
		
		return true;
	}

	return false;
}

void Game::updateScores() {
	SDL_Color scoreColor = {255, 255, 255};
	p1ScoreText.loadFromRenderedText(std::to_string(p1Score), scoreColor);
	p2ScoreText.loadFromRenderedText(std::to_string(p2Score), scoreColor);
}

void Game::displayWinText(Uint8 player) {
	SDL_Color scoreColor = {255, 255, 255};
	winningText.loadFromRenderedText("Player " + std::to_string(player) + " wins! Press R to restart.", scoreColor);
}