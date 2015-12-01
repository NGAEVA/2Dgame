#include <SGE.h>
using namespace SGE;

SGE_Sprite sprite;
Vector2 position;
bool quit = false;

class HelloGame : public Game
{
protected:
	virtual void OnInitializeImpl()
	{
		sprite.Load("optimus.jpg");
		position.x = 100.0f;
		position.y = 100.0f;
	}

	virtual void OnTerminateImpl()
	{
		sprite.Unload();
	}

	virtual bool OnUpdateImpl(float dt)
	{
		const float kSpeed = 100.0f;
		if (Input_IsKeyDown(Keys::RIGHT))
		{
			position.x += kSpeed * dt;
		}
		else if (Input_IsKeyDown(Keys::LEFT))
		{
			position.x -= kSpeed * dt;
		}

		if (Input_IsKeyDown(Keys::DOWN))
		{
			position.y += kSpeed * dt;
		}
		else if (Input_IsKeyDown(Keys::UP))
		{
			position.y -= kSpeed * dt;
		}

		if (Input_IsKeyPressed(Keys::ESCAPE))
		{
			quit = true;
		}
		return quit;
	}

	virtual void OnRenderImpl(float dt)
	{
		sprite.SetPosition(position);
		sprite.Render();
	}
};


Game* SGE_CreateGame()
{
	HelloGame* hg = new HelloGame();
	return hg;
}
