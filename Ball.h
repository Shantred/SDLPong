#pragma once
#include "Sprite.h"
class Ball : public Sprite
{
public:
	Ball(SDL_Renderer* renderer, int x, int y);
	~Ball();

	int velX;
	int velY;
	static const int BALL_HEIGHT = 20;
	static const int BALL_WIDTH = 20;
	const float BALL_SPEED = 2.0f;
	bool isColliding = false;

	enum BallState {
		BALL_ON_PADDLE,
		BALL_MOVING_LEFT,
		BALL_MOVING_RIGHT,
		BALL_OUT,
		TOTAL_BALL_STATES
	};
	
	void setVelocity(int velX, int velY);
	void setPosition(int x, int y);
	void changeVelocity(int velX, int velY);
	void reset(SDL_Rect &Paddle);
	void move(SDL_Rect &Paddle);
	bool handleCollision(SDL_Rect &paddle);
	BallState getState();
	BallState setState(BallState state);
	void launch(int directionX);

	BallState state;

private:
	SDL_Rect collider;
	
};

