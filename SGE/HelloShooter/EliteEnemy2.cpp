#include "EliteEnemy2.h"

EliteEnemy2::EliteEnemy2()
	: Enemy()
	, mHits(0)
{
}

EliteEnemy2::~EliteEnemy2()
{
}

void EliteEnemy2::Load()
{
	mSprite.Load("Ginyu.png");
	Enemy::Load();
}

void EliteEnemy2::Unload()
{
	mSprite.Unload();
}

bool EliteEnemy2::checkfire2()
{
	int firetimer;
	firetimer = Random_GetFloat(0.0f, 5.0f);

	if (firetimer == 1.0f && mActive)
	{
		return true;
	}

	return false;
}

bool EliteEnemy2::Update(float deltaTime)
{
	if (mActive)
	{
		mTimer -= deltaTime;
		if (mTimer <= 0.0f)
		{
			mVelocity.x *= 1.0f;
			mTimer = Random_GetFloat(0.0f, 1.5f);
		}

		mPosition += mVelocity * deltaTime;

		const float threshold = (float)mSprite.GetHeight();
		const float bottom = IniFile_GetInt("WinHeight", 600) + threshold;
		const int right = IniFile_GetInt("WinWidth", 600);

		if (mPosition.x < 0 || mPosition.x > right)
		{
			mVelocity.x *= -2;
		}

		if (mPosition.y > bottom)
		{
			Kill();
		}
	}

	return mActive;
}

void EliteEnemy2::Render()
{
	Enemy::Render();
}

void EliteEnemy2::Spawn()
{
	const float kWinWidth = (float)IniFile_GetInt("WinWidth", 800);
	const float threshold = 100.0f;
	const float x = Random_GetFloat(threshold, kWinWidth - threshold);
	const float y = -(float)mSprite.GetHeight();
	mPosition = Vector2(x, y);
	mVelocity = Vector2(100.0f, 100.0f);
	mTimer = Random_GetFloat(0.0f, 1.5f);
	mActive = true;
}

bool EliteEnemy2::Kill()
{
	++mHits;

	if (mHits >= IniFile_GetInt("EliteHitCount2", 1))
	{
		mActive = false;
	}

	return mActive;
}