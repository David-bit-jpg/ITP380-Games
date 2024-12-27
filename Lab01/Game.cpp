#include "Game.h"
Game::Game()
: mPaddle{0, 0, 0, 0}
, mBallVelocity{0, 0}
, mBall{0, 0, 0, 0}
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mIsRunning = false;
	mPreviousTime = 0;
	mTimeIncrease = 0;
	mDirection = 0;
	mPaddle.x = WALL_WIDTH;
	mPaddle.y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
	mBall.x = WINDOW_WIDTH / 2 - BALL_SIZE;
	mBall.y = WINDOW_HEIGHT / 2 - BALL_SIZE;
	mBallVelocity.x = BALL_VELOCITY_X;
	mBallVelocity.y = BALL_VELOCITY_Y;
}

Game::~Game()
{
	Shutdown();
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		return false;
	}
	mWindow = SDL_CreateWindow("mWindow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							   WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!mWindow)
	{
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1,
								   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		return false;
	}
	mIsRunning = true;
	return true;
}
void Game::Shutdown()
{
	mIsRunning = false;
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			mIsRunning = false;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	if (state[SDL_SCANCODE_W])
	{
		mDirection = 1;
	}
	else if (state[SDL_SCANCODE_S])
	{
		mDirection = -1;
	}
	else
	{
		mDirection = 0;
	}
}

void Game::UpdateGame()
{
	float mCurrentTime = 0;
	while (true)
	{
		mCurrentTime = static_cast<float>(SDL_GetTicks());
		mTimeIncrease = mCurrentTime - mPreviousTime;
		if (mTimeIncrease >= FRAME_FACTOR)
		{
			break;
		}
	}
	mPreviousTime = mCurrentTime;
	float deltaTime = mTimeIncrease / MILLISECOND_FACTOR;
	// SDL_Log("%f", deltaTime);
	if (deltaTime > DELTATIME_LIMIT)
	{
		deltaTime = DELTATIME_LIMIT;
	}

	if (mPaddle.y >= WALL_WIDTH && mPaddle.y <= BOTTOM_COLLIDE)
	{
		mPaddle.y -= static_cast<int>(deltaTime * MILLISECOND_FACTOR) * mDirection;
		if (mPaddle.y < WALL_WIDTH)
		{
			mPaddle.y = WALL_WIDTH;
		}
		if (mPaddle.y > BOTTOM_COLLIDE)
		{
			mPaddle.y = BOTTOM_COLLIDE;
		}
	}
	mBall.x += static_cast<int>(mBallVelocity.x * deltaTime);
	mBall.y += static_cast<int>(mBallVelocity.y * deltaTime); //move
	HitWall();
	HitPaddle();
}
void Game::HitWall()
{
	if (mBall.y >= WINDOW_HEIGHT - WALL_WIDTH - BALL_SIZE) //bottom
	{
		mBall.y = WINDOW_HEIGHT - WALL_WIDTH - BALL_SIZE;
		Accelerate();
		mBallVelocity.y = -mBallVelocity.y;
		// SDL_Log("Hit Bottom with %d, %d", mBall.x, mBall.y);
	}
	if (mBall.x >= WINDOW_WIDTH - WALL_WIDTH) //right
	{
		mBall.x = WINDOW_WIDTH - WALL_WIDTH;
		Accelerate();
		mBallVelocity.x = -mBallVelocity.x;
		// SDL_Log("Hit Right with %d, %d", mBall.x, mBall.y);
	}
	if (mBall.y <= WALL_WIDTH) //top
	{
		mBall.y = WALL_WIDTH;
		Accelerate();
		mBallVelocity.y = -mBallVelocity.y;
		// SDL_Log("Hit Top with %d, %d", mBall.x, mBall.y);
	}
}
void Game::Accelerate()
{
	if (mBallVelocity.x >= 0)
	{
		mBallVelocity.x += ACCELERATE_FACTOR;
	}
	else if (mBallVelocity.x < 0)
	{
		mBallVelocity.x -= ACCELERATE_FACTOR;
	}

	if (mBallVelocity.y >= 0)
	{
		mBallVelocity.y += ACCELERATE_FACTOR;
	}
	else if (mBallVelocity.y < 0)
	{
		mBallVelocity.y -= ACCELERATE_FACTOR;
	}
}
void Game::HitPaddle()
{
	if (mBall.x <= WALL_WIDTH + BALL_SIZE)
	{
		if ((mBall.y) >= (mPaddle.y - BALL_SIZE) && (mBall.y) <= (mPaddle.y + PADDLE_HEIGHT))
		{
			mBall.x = WALL_WIDTH + BALL_SIZE;
			Accelerate();
			mBallVelocity.x = -mBallVelocity.x;
		}
	}
	if (mBall.x <= ORIGIN_POINT)
	{
		mIsRunning = false;
	}
}

void Game::GenerateOutput()
{
	if (SDL_SetRenderDrawColor(mRenderer, 169, 169, 169, 255) != 0)
	{
		return;
	}

	if (SDL_RenderClear(mRenderer) != 0)
	{
		return;
	}
	SDL_Rect rectTop;
	rectTop.x = ORIGIN_POINT;
	rectTop.y = ORIGIN_POINT;
	rectTop.w = WINDOW_WIDTH;
	rectTop.h = WALL_WIDTH;
	SDL_SetRenderDrawColor(mRenderer, 34, 139, 34, 255);
	SDL_Rect rectRight;
	rectRight.x = WINDOW_WIDTH - WALL_WIDTH;
	rectRight.y = ORIGIN_POINT;
	rectRight.w = WALL_WIDTH;
	rectRight.h = WINDOW_HEIGHT;
	SDL_SetRenderDrawColor(mRenderer, 34, 139, 34, 255);
	SDL_Rect rectBottom;
	rectBottom.x = ORIGIN_POINT;
	rectBottom.y = WINDOW_HEIGHT - WALL_WIDTH;
	rectBottom.w = WINDOW_WIDTH;
	rectBottom.h = WALL_WIDTH;
	SDL_SetRenderDrawColor(mRenderer, 34, 139, 34, 255);
	if (SDL_RenderFillRect(mRenderer, &rectTop) != 0)
	{
		return;
	}
	if (SDL_RenderFillRect(mRenderer, &rectRight) != 0)
	{
		return;
	}
	if (SDL_RenderFillRect(mRenderer, &rectBottom) != 0)
	{
		return;
	}
	mPaddle.w = PADDLE_WIDTH;
	mPaddle.h = PADDLE_HEIGHT;
	SDL_SetRenderDrawColor(mRenderer, 153, 102, 204, 255);
	if (SDL_RenderFillRect(mRenderer, &mPaddle) != 0)
	{
		return;
	}
	mBall.w = BALL_SIZE;
	mBall.h = BALL_SIZE;
	SDL_SetRenderDrawColor(mRenderer, 255, 99, 71, 255);
	if (SDL_RenderFillRect(mRenderer, &mBall) != 0)
	{
		return;
	}
	SDL_RenderPresent(mRenderer);
}
