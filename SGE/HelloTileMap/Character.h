#ifndef INCLUDED_CHARACTER_H
#define INCLUDED_CHARACTER_H

#include <SGE.h>
using namespace SGE;

class Map;

class Character
{
public:
	Character();
	~Character();

	void Load1();
	void Load2();
	void Unload();
	void Update(float deltaTime, const Map& map);
	void Render();
	
	Rect GetBoundingBox() const;

	// Accessors
	const Vector2& GetPosition() const		{ return mPosition; }
	const Vector2& GetVelocity() const		{ return mVelocity; }
	const Vector2& GetOffset() const		{ return mOffset; }

	// Mutators
	void SetPosition(const Vector2& pos)	{ mPosition = pos; }
	void SetVelocity(const Vector2& vel)	{ mVelocity = vel; }
	void SetOffset(const Vector2& offset)	{ mOffset = offset; }
	

private:
	SGE_Sprite mSprite;

	Vector2 mPosition;
	Vector2 mVelocity;
	Vector2 mOffset;

	bool mJumping;
};

#endif // #ifndef INCLUDED_CHARACTER_H