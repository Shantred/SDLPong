#include "Paddle.h"

Paddle::Paddle(SDL_Renderer* renderer, int x, int y, int moveSpeed) : Sprite::Sprite(renderer, "paddle.png", x, y), MOVE_SPEED(moveSpeed), clipX(x)
{
	// Configure collider
	Paddle::collider.h = Paddle::PADDLE_HEIGHT;
	Paddle::collider.w = Paddle::PADDLE_WIDTH;

	// Set default positons
	Paddle::posX = x;
	Paddle::posY = y;
	Paddle::collider.x = x;
	Paddle::collider.y = y;
}

Paddle::~Paddle()
{
}


// Move should only be called when input to the left or right is received.
void Paddle::move(MoveDirection direction)
{
	// Always make sure the paddle is in the correct Y position
	Paddle::posX = Paddle::clipX;
	Paddle::collider.x = Paddle::clipX;

	// Before doing anything, make sure ball is not out of bounds.
	// When out of bounds, return it to the closest edge
	if (Paddle::posY < -5) {
		// By setting to this value, we're resetting the paddle to a valid
		// position while also preventing further movement to the left
		// This should prevent paddle jittering
		Paddle::posY = -1;
		Paddle::collider.y = -1;
	}

	if (Paddle::posY > 510) {
		Paddle::posY = 510;
		Paddle::collider.y = 510;
	}

	if (direction == DIRECTION_UP) {
		// Make sure paddle cannot move outside of the window
		if (Paddle::posY > 0) {
			Paddle::posY -= Paddle::MOVE_SPEED;
			Paddle::collider.y -= Paddle::MOVE_SPEED;

		}
	} else if (direction == DIRECTION_DOWN) {
		// Make sure paddle cannot move outside window
		if (Paddle::posY < 600) {
			Paddle::posY += Paddle::MOVE_SPEED;
			Paddle::collider.y += Paddle::MOVE_SPEED;
		}
	}
}

void Paddle::resetPosition()
{
	Paddle::posY = 280;
	Paddle::collider.y = 280;
}