#ifndef INCLUDED_GAMECONTEXT_H
#define INCLUDED_GAMECONTEXT_H

#include <SGE.h>
using namespace SGE;

class GameContext
{
public:
	GameContext();
	~GameContext();

	void SaveData(const char* pFilename);
	void LoadData(const char* pFilename);

	int GetLevel() const			{ return mLevel; }
	void SetLevel(int level)		{ mLevel = level; }

private:
	int mLevel;
};

#endif // #ifndef INCLUDED_GAMECONTEXT_H