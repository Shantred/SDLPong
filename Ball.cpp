#include "Ball.h"
#include <cmath>

Ball::Ball(SDL_Renderer* renderer, int x, int y) : Sprite::Sprite(renderer, "dot.png", x, y), velX(0), velY(0), state(BALL_MOVING_RIGHT)
{
	Ball::setVelocity(-3, 0);
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

// Reset position of ball to 5,5 and velocity to 1,1
void Ball::reset(SDL_Rect &paddle)
{
	printf("Ball has been reset \n");
	// Set velocity back to whatever default is
	//Ball::setVelocity(-2, -2);

	// Change state to be on paddle so we disable movement until released
	Ball::state = BALL_ON_PADDLE;

	Ball::velX = -5;
	Ball::velY = 5;
	
}

Ball::BallState Ball::getState()
{
	return Ball::state;
}

Ball::BallState Ball::setState(Ball::BallState state)
{
	return Ball::state = state;
}

void Ball::move(SDL_Rect &paddle)
{
	printf("=============================\n");
	printf("========MOVEMENT FRAME=======\n");
	printf("=============================\n");
	if (Ball::state != BALL_ON_PADDLE) {
		printf("Ball not on paddle. \n");

		// Handle collision before moving
		Ball::handleCollision(paddle);

		// Make sure the ball is still in the window
		// When outside of bounds, we fix this by flipping the velocity of the axis we're out of bounds from
		// When out of the right side, X will be > screen width
		if (Ball::posY > 580) {
			printf("Flipping ball velocity, 580 \n");
			Ball::velY *= -1;
		} else if (Ball::posY < 0) {
			printf("Flipping ball velocity, 0 \n");
			Ball::velY *= -1;
		}

		

		if (Ball::posX > 780) {
			printf("ball oob on the right \n");
			Ball::setState(Ball::BALL_OUT);
		} else if (Ball::posX < 0) {
			Ball::velX *= -1;
		}

		Ball::posX += Ball::velX;
		Ball::collider.x = Ball::posX;
		Ball::posY += Ball::velY;
		Ball::collider.y = Ball::posY;
	} else {
		printf("Ball on paddle\n");
		// When on the paddle, the ball should always move with and be centered on the paddle
		Ball::posX = paddle.x + Ball::BALL_HEIGHT + Ball::BALL_HEIGHT / 2;
		Ball::collider.x = Ball::posX;
		Ball::posY = paddle.y + (paddle.h / 2) - (Ball::BALL_HEIGHT / 2);
		Ball::collider.y = Ball::posY;
	}
	
}

bool Ball::handleCollision(SDL_Rect &paddle)
{
	// The ball has limited colliding states with the paddle. It may collide on the right, top or bottom faces of the paddle.
	// Note that this math is relative to the direction the ball is going. For now, we are only dealing with leftward movement.
	// First, we determine if the ball is within the paddle at all. To do this, we treat the paddle as a box and check
	// to see if either corner of the ball(since the ball is actually a square) is within it.
	// If the ball is within the paddle, its' x <= the x + width of the paddle and the y of the ball is between the paddle's y or y + paddle height.
	// We also need to check both left corners of the paddle on the Y axis to check for partial collision(such as what would happen
	// if the ball collided with the top or bottom edge of the paddle.

	printf("Ball Information: \n Sprite X, Y: <%i, %i>\n Collider X, Y: <%i, %i> \n Velocity X, Y: <%i, %i>\n isColliding: %i\n", Ball::posX, Ball::posY, Ball::collider.x, Ball::collider.y, Ball::velX, Ball::velY, Ball::isColliding);
	printf("Paddle Information: \n Collider X, Y: <%i, %i>\n", paddle.x, paddle.y);
	
	// Our collision checks are fairly straight-forward. We will check if the ball is at or beyond the paddle on the X axis.
	// Then check if the Y coordinate of the ball is BETWEEN the top and bottom of the paddle. We do this with a check at both
	// the top and the bottom of the ball collider so that we can detect edge collisions.
	// In order for collision to be true, one or both of those Y axis checks need to be true.
	// If both Y conditions are true, we have a head-on collision. If only one of them is true, we have a top or bottom collision.
	bool collideX = (Ball::collider.x < (paddle.x + paddle.w));
	bool collideYTop = (Ball::collider.y > paddle.y && Ball::collider.y < (paddle.y + paddle.h));
	bool collideYBot = ((Ball::collider.y + Ball::BALL_HEIGHT) > paddle.y && (Ball::collider.y + Ball::BALL_HEIGHT) < (paddle.y + paddle.h));
	if (collideX) {
		printf("collideX: %i, collideYTop: %i, collideYBot: %i\n", collideX, collideYTop, collideYBot);
	}

	// On the frame the collision first occurs, we set isColliding to true. As long as that is true, we do not apply
	// any additional collision effects and will wait for collideX to be false to set it back to false.
	if (!collideX && Ball::isColliding) {
		printf("Ball no longer colliding \n");
		Ball::isColliding = false;
	}	

	if (!Ball::isColliding && collideX && (collideYTop || collideYBot)) {
		Ball::isColliding = true;

		// To deflect the ball at different angles depending on where it lands, we need to determine where we landed on the paddle
		// These velocity values were found by tinkering with what felt/looked right.
		float distanceFromPaddleCenter = (paddle.y + (paddle.h / 2)) - (Ball::collider.y + (Ball::BALL_HEIGHT / 2));
		
		if (distanceFromPaddleCenter == 0) {
			Ball::velY = 3;
		} else if (distanceFromPaddleCenter > 0 && distanceFromPaddleCenter <= 15) {
			Ball::velY = -4;
		} else if (distanceFromPaddleCenter > 15) {
			Ball::velY = -5;
		} else if (distanceFromPaddleCenter < 0 && distanceFromPaddleCenter >= -15) {
			Ball::velY = 4;
		} else if (distanceFromPaddleCenter < -15) {
			Ball::velY = 5;
		}

		Ball::velX *= -1;

		printf("Ball Y velocity: %i\n", Ball::velY);
		printf("distnace from paddle center %i\n", distanceFromPaddleCenter);

		//======================
		// Stupid Code Warning
		//======================

		// Additionally, we want to deflect the ball at an angle.
		// We can do this by getting the distance of the ball from the middle of the paddle and converting that distance
		// to an angle. The paddle is intentionally 90px to make this easier.
		// We want at least a 15 degree change here. With a maximum distance of 45, this means the greatest deflection
		// should be 60 degrees.

		// The following is code that I was trying to bounce the ball at arbitrary velocities. 
		// This is a level of precision that is absolutely not necessary, but I wanted to figure this out
		// as a challenge to myself. While kind of cool and gratifying to have figured out the trig involved,
		// it doesn't even make the gameplay good or interesting. I've commented it out, but have left it in
		// to show my thought process and in case I ever want to come back to this idea.
		//float bounceAngle = 50; 
		//float bounceRadians = V * cos((2 * M_PI)/9) - 5 * sin((2 * M_PI)/9);
	
		printf("distance found to be %f\n", distanceFromPaddleCenter);
		//printf("bounce Angle %f\n", bounceAngle);
		//printf("bounce radians %f\n", bounceRadians);
		//printf("test for when x = -5 and y = 5: %f \n", (-5 * cos((50 * M_PI)/180) - -5 * sin((50 * M_PI)/180)));
		//printf("test for when x = -5 and y = 5: %f \n", (-5 * -sin((50 * M_PI)/180) + 5 * cos((50 * M_PI)/180)));

		//float newX = (Ball::velXf * cos((bounceAngle * M_PI) / 180)) - -(Ball::velYf * sin((bounceAngle * M_PI) / 180));
		//float newY = (Ball::velXf * -sin((bounceAngle * M_PI) / 180)) + (Ball::velYf * cos((bounceAngle * M_PI) / 180));
		//printf("newX: %f, newY: %f\n", newX, newY);
		//Ball::velX = Ball::velXf = newX;
		//Ball::velY = Ball::velYf = newY;

		//printf("Velocities are now %f, %f\n", Ball::velXf, Ball::velYf);
		//====================
		// End Stupid Code
		//====================

		
		// The paddle is intentionally 90px high to make this math easier. 
		
		return true;
	} else {
		return false;
	}

	return false;
}

void Ball::launch(int directionX)
{
	Ball::velX = directionX * 5;
	Ball::velY = 0;
	Ball::setState(Ball::BALL_MOVING_RIGHT);
	
}


//Velocity of the ball is -2, 0
//When the ball hits the paddle, 
// Try ceil(radians * velocity)