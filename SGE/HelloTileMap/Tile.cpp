#include "Tile.h"

Tile::Tile()
	: mPosition(0.0f, 0.0f)
	, mSize(0)
	, mType(0)
	, mWalkable(true)
{
}

Tile::~Tile()
{
}

Rect Tile::GetBoundingBox() const
{
	return Rect
	(
		mPosition.x,
		mPosition.y,
		mPosition.x + mSize,
		mPosition.y + mSize
	);
}