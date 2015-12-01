#ifndef INCLUDED_ENEMY_H
#define INCLUDED_ENEMY_H

#include <SGE.h>
using namespace SGE;

class Enemy
{
public:
	Enemy();
	virtual ~Enemy();

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual bool Update(float deltaTime) = 0;
	virtual void Render() = 0;
	
	virtual void Spawn() = 0;
	virtual bool checkfire() { return false; }
	virtual bool checkfire2() { return false; }
	virtual bool checkfire3() { return false; }
	virtual bool checkfire4() { return false; }
	virtual bool Kill() = 0;

	const Vector2& GetPosition() const	{ return mPosition; }
	bool IsActive() const				{ return mActive; }
	Circle GetBoundingCircle() const	{ return Circle(mPosition, mRadius); }

protected:
	SGE_Sprite mSprite;
	Vector2 mPosition;
	Vector2 mVelocity;
	float mTimer;
	float mRadius;
	bool mActive;
};

#endif // #ifndef INCLUDED_ENEMY_H