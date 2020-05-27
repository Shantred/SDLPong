#pragma once
#include "Sprite.h"
#include "Paddle.h"

extern SDL_Window* gameWindow;
extern SDL_Renderer* gameRenderer;
extern TTF_Font* gFont;

class Ball : public Sprite
{
public:
	Ball(int x, int y);
	~Ball();

	int velX;
	int velY;
	static const int BALL_HEIGHT = 20;
	static const int BALL_WIDTH = 20;
	const float BALL_SPEED = 2.0f;
	bool isColliding = false;

	enum BallState {
		BALL_MOVING,
		BALL_ON_PADDLE,
		BALL_OUT_RIGHT,
		BALL_OUT_LEFT,
		TOTAL_BALL_STATES
	};
	
	void setVelocity(int velX, int velY);
	void setPosition(int x, int y);
	void changeVelocity(int velX, int velY);
	void reset();
	void move(Paddle *paddle);
	BallState getState();
	BallState setState(BallState state);
	void launch();

	BallState state;
	SDL_Rect collider;
	
};

