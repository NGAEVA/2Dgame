#include "MinionEnemy.h"

MinionEnemy::MinionEnemy()
	: Enemy()
{
}

MinionEnemy::~MinionEnemy()
{
}

void MinionEnemy::Load()
{
	mSprite.Load("MONS_1392.png");
	Enemy::Load();
}

void MinionEnemy::Unload()
{
	mSprite.Unload();
}

bool MinionEnemy::Update(float deltaTime)
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
		
		const float threshold = (float)mSprite.GetHeight();
		const float bottom = IniFile_GetInt("WinHeight", 600) + threshold;
		if (mPosition.y > bottom)
		{
			Kill();
		}
	}

	return mActive;
}

void MinionEnemy::Render()
{
	Enemy::Render();
}

void MinionEnemy::Spawn()
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

bool MinionEnemy::Kill()
{
	mActive = false;
	return mActive;
}