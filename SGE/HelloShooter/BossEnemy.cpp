#include "BossEnemy.h"

BossEnemy::BossEnemy()
	: Enemy()
	, mHits(0)
{
}

BossEnemy::~BossEnemy()
{
}

void BossEnemy::Load()
{
	mSprite.Load("frieza.png");
	Enemy::Load();
}

void BossEnemy::Unload()
{
	mSprite.Unload();
}

bool BossEnemy::checkfire4()
{
	int firetimer;
	firetimer = Random_GetFloat(0.0f, 5.0f);

	if (firetimer == 1.0f && mActive)
	{
		return true;
	}

	return false;
}

bool BossEnemy::Update(float deltaTime)
{
	if (mActive)
	{
		const int threshold = mSprite.GetHeight();
		const int bottom = IniFile_GetInt("WinHeight", 800) +threshold;
		const int right = IniFile_GetInt("WinWidth", 600);

		mTimer -= deltaTime;
		if (mTimer <= 0.0f)
		{
			mVelocity.x += IniFile_GetFloat("BossSpeedUpFactorX", 1.0f);;
			mVelocity.y += IniFile_GetFloat("BossSpeedUpFactorY", 1.0f);;
			mTimer = Random_GetFloat(0.0f, 1.5f);
		}

		mPosition += mVelocity * deltaTime;

		//const float kYLimit = 200.0f;
		//if(mPosition.y > 0)
		//{
		//	//mPosition.y = 0.0f;
		//	mVelocity.y = IniFile_GetFloat("BossSpeedUpFactorY", 1.0f);
		//	mVelocity.x *= IniFile_GetFloat("BossSpeedUpFactorX", 1.0f);
		//}
		
		

		if(mPosition.x < 0 || mPosition.x > right)
		{
			mVelocity.x *= -1;
		}

		if (mPosition.y > bottom || mPosition.y < -threshold)
		{
			mVelocity.y *= -1;
		}

	}

	return mActive;
}

void BossEnemy::Render()
{
	Enemy::Render();
}

void BossEnemy::Spawn()
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

bool BossEnemy::Kill()
{
	++mHits;

	if(mHits >= IniFile_GetInt("BossHitCount", 1))
	{
		mActive = false;
	}
	
	return mActive;
}