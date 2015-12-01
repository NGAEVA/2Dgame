#ifndef INCLUDED_GAMEPLAYSTATE_H
#define INCLUDED_GAMEPLAYSTATE_H

#include "BaseState.h"
#include "Character.h"
#include "Map.h"

class GameplayState : public BaseState
{
public:
	GameplayState(GameContext& gc);
	~GameplayState();

	// Implements BaseState
	virtual void Load();
	virtual void Unload();
	virtual GameState Update(float deltaTime);
	virtual void Render();

private:
	SGE_Cursor mCursor;

	Character* mpCharacter;
	Map mMap;
};

#endif // #ifndef INCLUDED_GAMEPLAYSTATE_H