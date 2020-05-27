#include "Ball.h"
#include "Paddle.h"
#include <cmath>

// Ball is a Sprite subclass that provides movement functionality with handling for when the ball is out of bounds.
// The ball is technically square, though the sprite is round. We still use a square collider.
// TODO: Maybe make the collider smaller to match the actual dimensions of the white part of the ball to improve collisions accuracy.
Ball::Ball(int x, int y) : Sprite::Sprite("./assets/image/dot.png", x, y), velX(0), velY(0), state(BALL_ON_PADDLE)
{
	// We default velocity to 1 because we only initialize the ball once. 
	// When the game starts, we give P1 the ball first and point it
	// in the direction of p2.
	Ball::setVelocity(1, 0);
	Ball::setPosition(x, y);
}

Ball::~Ball()
{
}

void Ball::setVelocity(int velX, int velY)
{
	Ball::velX = velX;
	Ball::velY = velY;
}

void Ball::setPosition(int x, int y)
{
	Sprite::setPosition(x, y);
	Ball::collider.x = x;
	Ball::collider.y = y;
}

void Ball::changeVelocity(int velX, int velY)
{
	Ball::velX += velX;
	Ball::velY += velY;
}

// Resetting the ball simply resets the state of the ball to stop movement.
// Tracking to a paddle is handled in move().
void Ball::reset()
{
	printf("Ball has been reset \n");

	// Change state to be on paddle so we disable movement until released
	Ball::state = BALL_ON_PADDLE;
	// Ball::velX = -5;
	// Ball::velY = 5;
	
}

Ball::BallState Ball::getState()
{
	return Ball::state;
}

Ball::BallState Ball::setState(Ball::BallState state)
{
	return Ball::state = state;
}

// Moves the ball. When state is BALL_ON_PADDLE, the ball movement is centered against a provided paddle position.
// The paddle the ball tracks is dependent on where the ball went out of bounds.
void Ball::move(Paddle *paddle)
{
	printf("=============================\n");
	printf("========MOVEMENT FRAME=======\n");
	printf("=============================\n");
	if (Ball::state != BALL_ON_PADDLE) {
		printf("Ball not on paddle. \n");


		// Make sure the ball is still in the window
		// When outside of bounds, assuming this isn't a score, we fix this by flipping the velocity of the axis we're out of bounds from
		// When out of the right side, X will be > screen width
		if (Ball::posY > 590) {
			printf("Flipping ball velocity, 580 \n");
			Ball::velY *= -1;
		} else if (Ball::posY < 10) {
			printf("Flipping ball velocity, 0 \n");
			Ball::velY *= -1;
		}

		// Check for ball outs. These trigger a score in Game.cpp
		if (Ball::posX > 785) {
			Ball::setState(Ball::BALL_OUT_RIGHT);
		} else if (Ball::posX < 15) {
			Ball::setState(Ball::BALL_OUT_LEFT);
		}

		// If all else is good, move the ball colliders and positions.
		Ball::posX += Ball::velX;
		Ball::collider.x = Ball::posX;
		Ball::posY += Ball::velY;
		Ball::collider.y = Ball::posY;
	} else {
		printf("Ball on paddle\n");
		// When on the paddle, the ball should always move with and be centered on the paddle
		// checking for an X position over 500 tells us whether we are tracking p2 paddle.
		// This matters because the ball needs to be on the INSIDE of the court.
		if (Ball::posX < 500) {
			Ball::posX = paddle->collider.x + Ball::BALL_HEIGHT + Ball::BALL_HEIGHT / 2;
			Ball::collider.x = Ball::posX;
			Ball::posY = paddle->collider.y + (paddle->collider.h / 2) - (Ball::BALL_HEIGHT / 2);
			Ball::collider.y = Ball::posY;
		} else {
			Ball::posX = paddle->collider.x - Ball::BALL_HEIGHT - Ball::BALL_HEIGHT / 2;
			Ball::collider.x = Ball::posX;
			Ball::posY = paddle->collider.y + (paddle->collider.h / 2) - (Ball::BALL_HEIGHT / 2);
			Ball::collider.y = Ball::posY;
		}
		
	}
	
}

// Launches the ball straight to the left or right, depending on which paddle the ball is currently on.
void Ball::launch()
{
	Ball::velX *= 9;
	Ball::velY = 0;
	Ball::setState(Ball::BALL_MOVING);

}