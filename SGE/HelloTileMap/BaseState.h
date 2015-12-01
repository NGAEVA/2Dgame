#ifndef INCLUDED_BASESTATE_H
#define INCLUDED_BASESTATE_H

#include "GameContext.h"
#include "GameStates.h"

class BaseState
{
public:
	BaseState(GameContext& gc);
	virtual ~BaseState();

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual GameState Update(float deltaTime) = 0;
	virtual void Render() = 0;

protected:
	GameContext& mGameContext;
};

#endif // #ifndef INCLUDED_BASESTATE_H