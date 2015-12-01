#include "Fighter.h"

Fighter::Fighter()
	: mPosition(0.0f, 0.0f)
	, mSpeed(0.0f)
	, mActive(true)
{
}

Fighter::~Fighter()
{
}

void Fighter::Load()
{
	mSprite.Load("SonGoku.png");
	mSprite.SetScale(0.75f, 0.75f);
}

void Fighter::Unload()
{
	mSprite.Unload();
}

void Fighter::Update(float deltaTime)
{
	if (mActive)
	{
		const int left = 0;
		const int top = 0;
		const int right = IniFile_GetInt("WinWidth", 800);
		const int bottom = IniFile_GetInt("WinHeight", 600);

		int mouseMoveX = Input_GetMouseMoveX();
		int mouseMoveY = Input_GetMouseMoveY();

		mPosition.x += mouseMoveX;

		float x1 = mPosition.x + (mSpeed * deltaTime);
		if (x1 < right)
		{
			mPosition.x = x1;
		}

		float x2 = mPosition.x - (mSpeed * deltaTime);
		if (x2 > left)
		{
			mPosition.x = x2;
		}

		mPosition.y += mouseMoveY;

		float y1 = mPosition.y + (mSpeed * deltaTime);
		if (y1 < bottom)
		{
			mPosition.y = y1;
		}

		float y2 = mPosition.y - (mSpeed * deltaTime);
		if (y2 > top)
		{
			mPosition.y = y2;
		}

		/*if (Input_IsKeyDown(Keys::D))
		{
			float x = mPosition.x + (mSpeed * deltaTime);
			if (x < right)
			{
				mPosition.x = x;
			}
		}
		else if (Input_IsKeyDown(Keys::A))
		{
			float x = mPosition.x - (mSpeed * deltaTime);
			if (x > left)
			{
				mPosition.x = x;
			}
		}

		if (Input_IsKeyDown(Keys::S))
		{
			float y = mPosition.y + (mSpeed * deltaTime);
			if (y < bottom)
			{
				mPosition.y = y;
			}
		}
		else if (Input_IsKeyDown(Keys::W))
		{
			float y = mPosition.y - (mSpeed * deltaTime);
			if (y > top)
			{
				mPosition.y = y;
			}
		}*/
	}
}

void Fighter::Render()
{
	if (mActive)
	{
		const float kHalfWidth = mSprite.GetWidth() * 0.5f;
		const float kHalfHeight = mSprite.GetHeight() * 0.5f;
		const Vector2 renderPos(mPosition.x - kHalfWidth, mPosition.y - kHalfHeight);
		mSprite.SetPosition(renderPos);
		mSprite.Render();
	}
}

bool Fighter::Kill()
{
	mActive = false;
	return mActive;
}