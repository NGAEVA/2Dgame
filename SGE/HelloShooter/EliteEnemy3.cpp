#include "EliteEnemy3.h"

EliteEnemy3::EliteEnemy3()
	: Enemy()
	, mHits(0)
{
}

EliteEnemy3::~EliteEnemy3()
{
}

void EliteEnemy3::Load()
{
	mSprite.Load("Vegeta.png");
	Enemy::Load();
}

void EliteEnemy3::Unload()
{
	mSprite.Unload();
}

bool EliteEnemy3::checkfire3()
{
	int firetimer;
	firetimer = Random_GetFloat(0.0f, 5.0f);

	if (firetimer == 1.0f && mActive)
	{
		return true;
	}

	return false;
}

bool EliteEnemy3::Update(float deltaTime)
{
	if (mActive)
	{
		mTimer -= deltaTime;
		if (mTimer <= 0.0f)
		{
			mVelocity.x *= -1.0f;
			mTimer = Random_GetFloat(0.0f, 1.5f);
		}

		mPosition += mVelocity * deltaTime;

		const float kYLimit = 200.0f;
		if(mPosition.y > kYLimit)
		{
			mPosition.y = kYLimit;
			mVelocity.y = 0.0f;
			mVelocity.x *= 8.0f;
		}

		const float threshold = (float)mSprite.GetHeight();
		const float bottom = IniFile_GetInt("WinHeight", 600) + threshold;
		const int right = IniFile_GetInt("WinWidth", 600);

		if (mPosition.x < 0 || mPosition.x > right)
		{
			mVelocity.x *= -1;
		}

		if (mPosition.y > bottom)
		{
			Kill();
		}
	}

	return mActive;
}

void EliteEnemy3::Render()
{
	Enemy::Render();
}

void EliteEnemy3::Spawn()
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

bool EliteEnemy3::Kill()
{
	++mHits;

	if (mHits >= IniFile_GetInt("EliteHitCount3", 1))
	{
		mActive = false;
	}

	return mActive;
}