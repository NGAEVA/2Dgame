#ifndef INCLUDED_COLLIDABLE_H
#define INCLUDED_COLLIDABLE_H

#include <SGE.h>
using namespace SGE;

class Collidable
{
public:
	Collidable();
	~Collidable();

	void Load();
	void Unload();
	void Update(float deltaTime);
	void Render();
	
	void Fire(const Vector2& pos, const Vector2& vel);
	void Collide(const Vector2& pos);
	void Kill();

	bool IsActive() const				{ return mActive; }
	Circle GetBoundingCircle() const	{ return Circle(mPosition, 10.0f); }

private:
	SGE_Sprite mSprite;
	Vector2 mPosition;
	Vector2 mVelocity;
	bool mActive;
};

#endif // #ifndef INCLUDED_COLLIDABLE_H