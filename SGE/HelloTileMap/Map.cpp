#include "Map.h"

Map::Map()
	: mSprites(nullptr)
	, mTiles(nullptr)
	, mOffset(0.f, 0.f)
	, mColumns(0)
	, mRows(0)
	, mTileSize(0)
	, mSpriteCount(0)
{
	// Empty
}

Map::~Map()
{
	// Empty
}

void Map::Load(const char* pLevelFile, const char* pTexturePack)
{
	// Clean up before we start loading anything
	Unload();

	// Load level data
	if (!LoadLevel(pLevelFile))
	{
		Unload();
		return;
	}

	// Load texture pack
	if (!LoadTexturePack(pTexturePack))
	{
		Unload();
		return;
	}
}

void Map::Unload()
{
	if (mSprites != nullptr)
	{
		for (int i = 0; i < mSpriteCount; ++i)
		{
			mSprites[i].Unload();
		}
		delete[] mSprites;
		mSprites = nullptr;
	}
	if (mTiles != nullptr)
	{
		delete[] mTiles;
		mTiles = nullptr;
	}

	// Reset params
	mColumns = 0;
	mRows = 0;
	mTileSize = 0;
	mSpriteCount = 0;
}

void Map::Update(float deltaTime)
{
	// Do nothing
}

void Map::Render()
{
	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			const int index = x + (y * mColumns);
			const Tile& tile = mTiles[index];
			const Vector2& pos = tile.GetPosition();
			const int type = tile.GetType();

			SGE_Sprite& sprite = mSprites[type];
			sprite.SetPosition(pos + mOffset);
			sprite.Render();

			if (!tile.IsWalkable())
			{
				Rect bb = tile.GetBoundingBox();
				Graphics_DebugRect(bb + mOffset, 0x0000ff);
			}
		}
	}
}

Rect Map::GetBoundingBoxFromSegment(const LineSegment& line) const
{
	// Validate the line segment
	if (line.from.x < 0.0f || line.from.x > GetWidth() ||
		line.from.y < 0.0f || line.from.y > GetHeight() ||
		line.to.x < 0.0f || line.to.x > GetWidth() ||
		line.to.y < 0.0f || line.to.y > GetHeight())
	{
		// Line segment is out of bound!
		return Rect();
	}
	
	// Convert position to indices
	const int fromX = static_cast<int>(line.from.x) / mTileSize;
	const int fromY = static_cast<int>(line.from.y) / mTileSize;
	const int toX = static_cast<int>(line.to.x) / mTileSize;
	const int toY = static_cast<int>(line.to.y) / mTileSize;

	// Calculate tile count
	const int countX = toX - fromX + 1;
	const int countY = toY - fromY + 1;
	
	// Get region
	Rect region;
	for (int y = 0; y < countY; ++y)
	{
		for (int x = 0; x < countX; ++x)
		{
			const int index = (fromX + x) + ((fromY + y) * mColumns);
			if (!mTiles[index].IsWalkable())
			{
				region += mTiles[index].GetBoundingBox();
			}
		}
	}
	return region;
}

void Map::SetTile(int screenX, int screenY, int type)
{
	const int x = (screenX - (int)mOffset.x) / mTileSize;
	const int y = (screenY - (int)mOffset.y) / mTileSize;
	const int index = x + (y * mColumns);
	type = type % mSpriteCount;
	mTiles[index].SetType(type);

	mTiles[index].SetWalkable(type == 0);
}

int Map::GetTile(int screenX, int screenY)
{
	const int x = (screenX - (int)mOffset.x) / mTileSize;
	const int y = (screenY - (int)mOffset.y) / mTileSize;
	const int index = x + (y * mColumns);
	return mTiles[index].GetType();
}

bool Map::LoadLevel(const char* pLevelFile)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, pLevelFile, "r");

	// Read map dimensions
	fscanf_s(pFile, "%*s %d", &mColumns);
	fscanf_s(pFile, "%*s %d", &mRows);
	fscanf_s(pFile, "%*s %d", &mTileSize);

	// Create tile buffer
	mTiles = new Tile[mColumns * mRows];
	
	// Parse tile data
	fgetc(pFile);
	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			const int index = x + (y * mColumns);
			Tile& tile = mTiles[index];
			tile.SetPosition(Vector2((float)x * mTileSize, (float)y * mTileSize));
			tile.SetSize(mTileSize);
			tile.SetType(fgetc(pFile) - '0');
			tile.SetWalkable(tile.GetType() == 0);
		}
		fgetc(pFile);
	}

	// Close file
	fclose(pFile);
	pFile = nullptr;

	return true;
}

bool Map::LoadTexturePack(const char* pTexturePack)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, pTexturePack, "r");

	// Get number of textures
	fscanf_s(pFile, "%*s %d", &mSpriteCount);

	// Create sprite pool
	mSprites = new SGE_Sprite[mSpriteCount];

	// Load textures
	for (int i = 0; i < mSpriteCount; ++i)
	{
		char buffer[128];
		fscanf_s(pFile, "%s", buffer, 128);
		mSprites[i].Load(buffer);
	}

	// Close file
	fclose(pFile);
	pFile = nullptr;

	return true;
}