#include "Character.h"
#include "Map.h"

Character::Character()
	: mPosition(0.0f, 0.0f)
	, mVelocity(0.0f, 0.0f)
	, mOffset(0.f, 0.f)
	, mJumping(true)
{
}

Character::~Character()
{
}

void Character::Load1()
{
	mSprite.Load("mario_idle_01.png");
}

void Character::Load2()
{
	mSprite.Load("mario_idle_RIGHT.png");
}
void Character::Unload()
{
	mSprite.Unload();
}

void Character::Update(float deltaTime, const Map& map)
{
	const float kSpeed = 300.0f;

	// HANDLE LEFT/RIGHT MOVEMENT
	if (Input_IsKeyDown(Keys::RIGHT))
	{
		mVelocity.x = kSpeed * deltaTime;
	}
	else if (Input_IsKeyDown(Keys::LEFT))
	{
		mVelocity.x = -kSpeed * deltaTime;
	}
	else
	{
		mVelocity.x = 0.0f;
	}

	// Check collision
	Rect bb = GetBoundingBox();
	Rect newbb = bb + Vector2(mVelocity.x, 0.0f);
	Rect rightbb = map.GetBoundingBoxFromSegment(newbb.GetRightSegment());
	Rect leftbb = map.GetBoundingBoxFromSegment(newbb.GetLeftSegment());
	
	// Right collision
	if (mVelocity.x > 0.0f && rightbb.IsValid())
	{
		mPosition.x += (int)(rightbb.min.x - bb.max.x) - 1.0f;
	}
	// Left collision
	else if (mVelocity.x < 0.0f && leftbb.IsValid())
	{
		mPosition.x += (int)(leftbb.max.x - bb.min.x) + 1.0f;
	}
	else
	{
		mPosition.x += (int)mVelocity.x;
	}


	// HANDLE UP/DOWN MOVEMENT
	if (Input_IsKeyPressed(Keys::UP) && !mJumping)
	{
		mVelocity.y = -18.0f;	// Jumping force
		mJumping = true;
	}
	else
	{
		mVelocity.y += 10.0f * deltaTime;	// Gravity
	}

	// Check collision
	newbb = bb + Vector2(0.0f, mVelocity.y);
	Rect bottombb = map.GetBoundingBoxFromSegment(newbb.GetBottomSegment());
	Rect topbb = map.GetBoundingBoxFromSegment(newbb.GetTopSegment());

	// Bottom collision
	if (mVelocity.y > 0.0f && bottombb.IsValid())
	{
		mPosition.y += (int)(bottombb.min.y - bb.max.y) - 1.0f;
		mVelocity.y = 0.0f;
		mJumping = false;
	}
	// Top collision
	else if (mVelocity.y < 0.0f && topbb.IsValid())
	{
		mPosition.y += (int)(topbb.max.y - bb.min.y) + 1.0f;
		mVelocity.y = 0.0f;
	}
	else
	{
		mPosition.y += (int)mVelocity.y;
	}
}

void Character::Render()
{
	const int kTextureWidth = mSprite.GetWidth();
	const int kTextureHeight = mSprite.GetHeight();
	const Vector2 pos(mPosition.x - (kTextureWidth * 0.5f), mPosition.y - kTextureHeight);
	mSprite.SetPosition(pos + mOffset);
	mSprite.Render();
	
	Rect bb = GetBoundingBox();
	Graphics_DebugRect(bb + mOffset, 0xff0000);
}

Rect Character::GetBoundingBox() const
{
	return Rect
	(
		mPosition.x - 16.0f,
		mPosition.y - 56.0f,
		mPosition.x + 16.0f,
		mPosition.y
	);
}