#include "Paddle.h"

// Paddle is a sprite subclass that provided movement functionality with clipping to prevent the sprite
// from moving out of bounds. Also includes a collider that is used for checking collisions. In this case,
// a collider is not strictly necessary since the collider is the same size as the sprite, but to ensure
// some amount of standardization, we include it anyway. There are better ways to handle this, which may be
// a future TODO.
Paddle::Paddle(int x, int y, int moveSpeed) : Sprite::Sprite("paddle.png", x, y), MOVE_SPEED(moveSpeed), clipX(x)
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


// Called by Game.cpp when control keys are pressed.
void Paddle::move(MoveDirection direction)
{
	// Always make sure the paddle is in the correct X position
	Paddle::posX = Paddle::clipX;
	Paddle::collider.x = Paddle::clipX;

	// Before doing anything, make sure paddle is not out of bounds.
	// When out of bounds, return it to the closest edge
	if (Paddle::posY < -5) {
		// By setting to this value, we're resetting the paddle to a valid
		// position while also preventing further movement to the top
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