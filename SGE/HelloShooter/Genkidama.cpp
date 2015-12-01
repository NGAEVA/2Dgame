#include "Genkidama.h"

Genkidama::Genkidama()
	: mPosition(0.0f, 0.0f)
	, mVelocity(0.0f, 0.0f)
	, mActive(false)
{
}

Genkidama::~Genkidama()
{
}

void Genkidama::Load()
{
	mSprite.Load("shield.png");
}

void Genkidama::Unload()
{
	mSprite.Unload();
}

void Genkidama::Update(float deltaTime)
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

void Genkidama::Render()
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

void Genkidama::Fire(const Vector2& pos, const Vector2& vel)
{
	mPosition = pos;
	mVelocity = vel;
	mActive = true;
}


void Genkidama::Kill()
{
	mActive = false;
}