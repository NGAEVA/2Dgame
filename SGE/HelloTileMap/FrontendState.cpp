#include "FrontendState.h"

FrontendState::FrontendState(GameContext& gc)
	: BaseState(gc)
{
}

FrontendState::~FrontendState()
{
}

void FrontendState::Load()
{
	mCursor.Load("sword.png");
	mFont.Load(72);

	float posY = 400.0f;

	mStartButton.Load("button_on.png", "button_off.png");
	mStartButton.SetPosition(100.0f, posY);
	mStartButton.SetText("START", 255, 255, 255);

	posY += 50.0f;

	mLevelButton1.Load("button_on.png", "button_off.png");
	mLevelButton1.SetPosition(100.0f, posY);
	mLevelButton1.SetText("Level 1", 255, 255, 255);

	posY += 50.0f;

	mLevelButton2.Load("button_on.png", "button_off.png");
	mLevelButton2.SetPosition(100.0f, posY);
	mLevelButton2.SetText("Level 2", 255, 255, 255);

	posY += 50.0f;

	mLevelButton3.Load("button_on.png", "button_off.png");
	mLevelButton3.SetPosition(100.0f, posY);
	mLevelButton3.SetText("Level 3", 255, 255, 255);

	posY += 50.0f;

	mQuitButton.Load("button_on.png", "button_off.png");
	mQuitButton.SetPosition(100.0f, posY);
	mQuitButton.SetText("QUIT", 255, 255, 255);
}

void FrontendState::Unload()
{
	mCursor.Unload();
	mFont.Unload();
	mStartButton.Unload();
	mLevelButton1.Unload();
	mLevelButton2.Unload();
	mLevelButton3.Unload();
	mQuitButton.Unload();
}

GameState FrontendState::Update(float deltaTime)
{
	mCursor.Update(deltaTime);
	mStartButton.Update(deltaTime);
	mLevelButton1.Update(deltaTime);
	mLevelButton2.Update(deltaTime);
	mLevelButton3.Update(deltaTime);
	mQuitButton.Update(deltaTime);

	GameState result = GameState::Invalid;
	if (mStartButton.IsPressed())
	{
		result = GameState::Gameplay;
	}
	else if (mLevelButton1.IsPressed())
	{
		mGameContext.SetLevel(0);
	}
	else if (mLevelButton2.IsPressed())
	{
		mGameContext.SetLevel(1);
	}
	else if (mLevelButton3.IsPressed())
	{
		mGameContext.SetLevel(2);
	}
	else if (mQuitButton.IsPressed())
	{
		result = GameState::Quit;
	}
	return result;
}

void FrontendState::Render()
{
	mFont.Print("ZELDA GAME", 200, 200);
	mStartButton.Render();
	mLevelButton1.Render();
	mLevelButton2.Render();
	mLevelButton3.Render();
	mQuitButton.Render();
	mCursor.Render();
}