#pragma once
#include "Sprite.h"

extern SDL_Window* gameWindow;
extern SDL_Renderer* gameRenderer;
extern TTF_Font* gFont;

class Paddle : public Sprite
{
public:
	enum MoveDirection {
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_NONE,
		TOTAL_DIRECTIONS
	};

	const int MOVE_SPEED;
	static const int PADDLE_HEIGHT = 90;
	static const int PADDLE_WIDTH = 20;

	SDL_Rect collider;

	Paddle(int x, int y, int moveSpeed = 0);
	~Paddle();

	void reset();
	void move(MoveDirection direction);
	void resetPosition();

private:
	const int clipX;
};

