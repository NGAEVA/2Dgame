  #include <SGE.h>
using namespace SGE;

#include "FrontendState.h"
#include "GameplayState.h"

BaseState* currentState = nullptr;
GameState nextState = GameState::Invalid;
GameContext gameContext;


class TileMapGame : public Game
{
protected:
	virtual void OnInitializeImpl()
	{
		currentState = new FrontendState(gameContext);
		currentState->Load();
	}

	virtual void OnTerminateImpl()
	{
		currentState->Unload();
		delete currentState;
		currentState = nullptr;
	}

	virtual bool OnUpdateImpl(float dt)
	{
		// Check for state change
		if (nextState != GameState::Invalid)
		{
			currentState->Unload();
			delete currentState;
			currentState = nullptr;

			switch (nextState)
			{
			case GameState::Frontend:
				currentState = new FrontendState(gameContext);
				break;
			case GameState::Gameplay:
				currentState = new GameplayState(gameContext);
				break;
			}

			currentState->Load();
		}

		nextState = currentState->Update(dt);
		return (nextState == GameState::Quit);
	}

	virtual void OnRenderImpl(float dt)
	{
		currentState->Render();
	}
};


Game* SGE_CreateGame()
{
	TileMapGame* game = new TileMapGame();
	return game;
}