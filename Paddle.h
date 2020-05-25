#pragma once
#include "Sprite.h"
class Paddle : public Sprite
{
public:
	enum MoveDirection {
		DIRECTION_LEFT,
		DIRECTION_RIGHT,
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_NONE,
		TOTAL_DIRECTIONS
	};

	const int MOVE_SPEED;
	static const int PADDLE_HEIGHT = 90;
	static const int PADDLE_WIDTH = 20;

	SDL_Rect collider;

	Paddle(SDL_Renderer* renderer, int x, int y, int moveSpeed = NULL);
	~Paddle();

	void reset();
	void move(MoveDirection direction);
	void resetPosition();

private:
	const int clipX;
};

