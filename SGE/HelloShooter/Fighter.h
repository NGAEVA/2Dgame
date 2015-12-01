#ifndef INCLUDED_FIGHTER_H
#define INCLUDED_FIGHTER_H

#include <SGE.h>
using namespace SGE;

class Fighter
{
public:
	Fighter();
	~Fighter();

	void Load();
	void Unload();
	void Update(float deltaTime);
	void Render();

	bool Kill();
	
	void SetPosition(const Vector2& pos)	{ mPosition = pos; }
	void SetSpeed(float speed)				{ mSpeed = speed; }
	
	const Vector2& GetPosition() const		{ return mPosition; }
	Circle GetBoundingCircle() const		{ return Circle(mPosition, 50.0f); }
	bool IsActive() const					{ return mActive; }

private:
	SGE_Sprite mSprite;
	Vector2 mPosition;
	float mSpeed;
	bool mActive;
};

#endif // #ifndef INCLUDED_FIGHTER_H