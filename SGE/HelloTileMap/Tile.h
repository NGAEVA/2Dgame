#ifndef INCLUDED_TILE_H
#define INCLUDED_TILE_H

#include <SGE.h>
using namespace SGE;

class Tile
{
public:
	Tile();
	~Tile();

	Rect GetBoundingBox() const;
	
	const Vector2& GetPosition() const		{ return mPosition; }
	int GetSize() const						{ return mSize; }
	int GetType() const						{ return mType; }
	bool IsWalkable() const					{ return mWalkable; }

	void SetPosition(const Vector2& pos)	{ mPosition = pos; }
	void SetSize(int size)					{ mSize = size; }
	void SetType(int type)					{ mType = type; }
	void SetWalkable(bool walkable)			{ mWalkable = walkable; }

private:
	Vector2 mPosition;
	int mSize;
	int mType;
	bool mWalkable;
};

#endif // #ifndef INCLUDED_TILE_H