#include "EliteEnemy.h"

EliteEnemy::EliteEnemy()
	: Enemy()
	, mHits(0)
{
}

EliteEnemy::~EliteEnemy()
{
}

void EliteEnemy::Load()
{
	mSprite.Load("Dodoria.png");
	Enemy::Load();
}

void EliteEnemy::Unload()
{
	mSprite.Unload();
}

bool EliteEnemy::checkfire()
{
	int firetimer;
	firetimer = Random_GetFloat(0.0f,5.0f);

	if(firetimer == 1.0f && mActive)
	{
		return true;
	}

	return false;
}

bool EliteEnemy::Update(float deltaTime)
{
	if (mActive)
	{
		mTimer -= deltaTime;
		if (mTimer <= 0.0f)
		{
			mVelocity.x *= -1.0f;
			mVelocity.y *= 1.5f;
			mTimer = Random_GetFloat(0.0f, 1.5f);
		}

		mPosition += mVelocity * deltaTime;
		
		const float threshold = (float)mSprite.GetHeight();
		const float bottom = IniFile_GetInt("WinHeight", 600) + threshold;

		if (mPosition.y > bottom)
		{
			Kill();
		}
	}

	return mActive;
}

void EliteEnemy::Render()
{
	Enemy::Render();
}

void EliteEnemy::Spawn()
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

bool EliteEnemy::Kill()
{
	++mHits;

	if(mHits >= IniFile_GetInt("EliteHitCount", 1))
	{
		mActive = false;
	}
	
	return mActive;
}