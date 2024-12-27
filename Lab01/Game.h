#pragma once
#include "SDL2/SDL.h"
class Game
{
private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mIsRunning;
	float mTimeIncrease;
	float mPreviousTime;
	int mDirection;
	SDL_Rect mPaddle;
	SDL_Point mBallVelocity;
	SDL_Rect mBall;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void HitWall();
	void HitPaddle();
	void Accelerate();
	const int WINDOW_WIDTH = 1024;
	const int WINDOW_HEIGHT = 768;
	const int WALL_WIDTH = 20;
	const int PADDLE_WIDTH = 10;
	const int PADDLE_HEIGHT = 100;
	const int BALL_SIZE = 10;
	const int BALL_VELOCITY_X = 400;
	const int BALL_VELOCITY_Y = 400;
	const int ORIGIN_POINT = 0;
	const float MILLISECOND_FACTOR = 1000.0f;
	const int ACCELERATE_FACTOR = 7;
	const int FRAME_FACTOR = 16;
	const float DELTATIME_LIMIT = 0.033f;
	const int BOTTOM_COLLIDE = WINDOW_HEIGHT - WALL_WIDTH - PADDLE_HEIGHT;

public:
	Game();
	~Game();
	bool Initialize();
	void Shutdown();
	void RunLoop();
};
