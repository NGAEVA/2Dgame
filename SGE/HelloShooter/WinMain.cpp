#include <SGE.h>
using namespace SGE;

#include "World.h"
#include "World2.h"
#include "UI.h"

class ShooterGame : public Game
{
protected:
	virtual void OnInitializeImpl()
	{
		ui.Load();
		mWorld.Initialize();
		m2World.Initialize();

		ui.Activate();
	}

	virtual void OnTerminateImpl()
	{
		mWorld.Terminate();
		m2World.Terminate();
		ui.Unload();
	}

	virtual bool OnUpdateImpl(float dt)
	{
		GameState state = ui.Update(dt);
		if(state != GameState::Invalid)
		{
			if(state == GameState::Gameplay)
			{
				mWorld.Activate();
				ui.Deactivate();
			}

			if (state == GameState::Gameplay2)
			{
				m2World.Activate();
				ui.Deactivate();
			}
			if(state == GameState::Quit)
			{
				return true;
			}
		}

		mWorld.Update(dt);
		m2World.Update(dt);

		if (Input_IsKeyPressed(Keys::ESCAPE))
		{
			return true;
		}
		return false;
	}

	virtual void OnRenderImpl(float dt)
	{
		ui.Render();
		mWorld.Render(dt);
		m2World.Render(dt);
	}

private:
	World mWorld;
	World2 m2World;
	UI ui;
};


Game* SGE_CreateGame()
{
	ShooterGame* sg = new ShooterGame();
	return sg;
}