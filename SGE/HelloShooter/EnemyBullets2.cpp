#include "EnemyBullets2.h"

EnemyBullets2::EnemyBullets2()
	: mPosition(0.0f, 0.0f)
	, mVelocity(0.0f, 0.0f)
	, mActive(false)
{
}

EnemyBullets2::~EnemyBullets2()
{
}

void EnemyBullets2::Load()
{
	mSprite.Load("shield.png");
}

void EnemyBullets2::Unload()
{
	mSprite.Unload();
}

void EnemyBullets2::Update(float deltaTime)
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

void EnemyBullets2::Render()
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

void EnemyBullets2::Fire(const Vector2& pos, const Vector2& vel)
{
	mPosition = pos;
	mVelocity = vel;
	mActive = true;
}


void EnemyBullets2::Kill()
{
	mActive = false;
}