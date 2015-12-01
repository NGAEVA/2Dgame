#include "Enemy.h"

Enemy::Enemy()
	: mPosition(0.0f, 0.0f)
	, mVelocity(0.0f, 0.0f)
	, mTimer(0.0f)
	, mActive(false)
{
}

Enemy::~Enemy()
{
}

void Enemy::Load()
{
	mRadius = mSprite.GetWidth() / 2.f;
}

void Enemy::Render()
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