#include "Collidable.h"

Collidable::Collidable()
	: mPosition(0.0f, 0.0f)
	, mVelocity(0.0f, 0.0f)
	, mActive(false)
{
}

Collidable::~Collidable()
{
}

void Collidable::Load()
{
	mSprite.Load("bullet2.png");
}

void Collidable::Unload()
{
	mSprite.Unload();
}

void Collidable::Update(float deltaTime)
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

void Collidable::Render()
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

void Collidable::Fire(const Vector2& pos, const Vector2& vel)
{
	mPosition = pos;
	mVelocity = vel;
	mActive = true;
}

void Collidable::Collide(const Vector2& pos)
{
	const float dt = 1.f/60.f;

	mVelocity = SGE::Physics_Response(dt, mPosition, pos, mVelocity);
}

void Collidable::Kill()
{
	mActive = false;
}