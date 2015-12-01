#include "EnemyBullets1.h"

EnemyBullets1::EnemyBullets1()
	: mPosition(0.0f, 0.0f)
	, mVelocity(0.0f, 0.0f)
	, mActive(false)
{
}

EnemyBullets1::~EnemyBullets1()
{
}

void EnemyBullets1::Load()
{
	mSprite.Load("bullet2.png");
}

void EnemyBullets1::Unload()
{
	mSprite.Unload();
}

void EnemyBullets1::Update(float deltaTime)
{
	if (mActive)
	{
		mPosition += mVelocity * deltaTime;
		
		const float threshold = mSprite.GetWidth() * 0.5f;
		const float left = -threshold;
		const float top = -threshold;
		const float right = IniFile_GetInt("WinWidth", 800) + threshold;
		const float bottom = IniFile_GetInt("WinHeight", 600) + threshold;
		const Rect boundary(left, top, right, bottom);
		if (!Math_PointInRect(mPosition, boundary))
		{
			Kill();
		}
	}
}

void EnemyBullets1::Render()
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

void EnemyBullets1::Fire(const Vector2& pos, const Vector2& vel)
{
	mPosition = pos;
	mVelocity = vel;
	mActive = true;
}

void EnemyBullets1::Kill()
{
	mActive = false;
}