#include "UI.h"

UI::UI()
{
	mActive = false;
}

UI::~UI()
{
}

void UI::Activate()
{
	mActive = true;
}

void UI::Deactivate()
{
	mActive = false;
}

void UI::Load()
{
	mCursor.Load("sword.png");
	mFont.Load(72);
	background.Load("namek_water.jpg");
	music.Load("music1.mp3");
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

	/*posY += 50.0f;

	mLevelButton3.Load("button_on.png", "button_off.png");
	mLevelButton3.SetPosition(100.0f, posY);
	mLevelButton3.SetText("Level 3", 255, 255, 255);*/

	posY += 50.0f;

	mQuitButton.Load("button_on.png", "button_off.png");
	mQuitButton.SetPosition(100.0f, posY);
	mQuitButton.SetText("QUIT", 255, 255, 255);
}

void UI::Unload()
{
	mCursor.Unload();
	mFont.Unload();
	background.Unload();
	music.Unload();
	mStartButton.Unload();
	mLevelButton1.Unload();
	mLevelButton2.Unload();
	//mLevelButton3.Unload();
	mQuitButton.Unload();
}

GameState UI::Update(float deltaTime)
{
	GameState result = GameState::Invalid;
	if(mActive)
	{
		music.Play();
		mCursor.Update(deltaTime);
		mStartButton.Update(deltaTime);
		background.Update(deltaTime);
		mLevelButton1.Update(deltaTime);
		mLevelButton2.Update(deltaTime);
		//mLevelButton3.Update(deltaTime);
		mQuitButton.Update(deltaTime);

		if (mStartButton.IsPressed())
		{
			result = GameState::Gameplay;
		}
		else if (mLevelButton1.IsPressed())
		{
			result = GameState::Gameplay;
		}
		else if (mLevelButton2.IsPressed())
		{
			result = GameState::Gameplay2;
		}
		/*else if (mLevelButton3.IsPressed())
		{
			mGameContext.SetLevel(2);
		}*/
		else if (mQuitButton.IsPressed())
		{
			result = GameState::Quit;
		}
	}
	return result;
}

void UI::Render()
{
	if(mActive)
	{
		background.Render();
		mFont.Print("Sun Goku", 200, 200);
		mFont.SetColor(0,120,168);
		mStartButton.Render();
		mLevelButton1.Render();
		mLevelButton2.Render();
		//mLevelButton3.Render();
		mQuitButton.Render();
		mCursor.Render();
		
	}
}