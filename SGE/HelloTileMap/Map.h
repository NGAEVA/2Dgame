#ifndef INCLUDED_MAP_H
#define INCLUDED_MAP_H

#include <SGE.h>
using namespace SGE;

#include "NonCopyable.h"
#include "Tile.h"

class Map : private NonCopyable
{
public:
	Map();
	~Map();

	void Load(const char* pLevelFile, const char* pTexturePack);
	void Unload();
	void Update(float deltaTime);
	void Render();

	Rect GetBoundingBoxFromSegment(const LineSegment& line) const;

	int GetWidth() const	{ return mColumns * mTileSize; }
	int GetHeight() const	{ return mRows * mTileSize; }
	
	void SetTile(int screenX, int screenY, int type);
	int GetTile(int screenX, int screenY);

	void SetOffset(const Vector2& offset)	{ mOffset = offset; }
	Vector2 GetOffset() const				{ return mOffset; }

private:
	bool LoadLevel(const char* pLevelFile);
	bool LoadTexturePack(const char* pTexturePack);

	SGE_Sprite* mSprites;
	Tile* mTiles;

	Vector2 mOffset;

	int mColumns;
	int mRows;
	int mTileSize;
	int mSpriteCount;
};
	
#endif // #ifndef INCLUDED_MAP_H