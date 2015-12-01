#include "GameplayState.h"

GameplayState::GameplayState(GameContext& gc)
	: BaseState(gc)
	, mpCharacter(nullptr)
{
}

GameplayState::~GameplayState()
{
}

void GameplayState::Load()
{
	mCursor.Load("sword.png");

	const char* levels[] =
	{
		"level01.txt",
		"level02.txt",
		"level03.txt"
	};
	const char* texturePacks[] =
	{
		"texturepack01.txt",
		"texturepack02.txt",
		"texturepack03.txt"
	};

	const int currentLevel = mGameContext.GetLevel();
	mMap.Load(levels[currentLevel], texturePacks[currentLevel]);

	if (currentLevel == 2)
	{
		mpCharacter = new Character();
		mpCharacter->Load2();
		mpCharacter->SetPosition(Vector2(100.0f, 100.0f));
	}
}

void GameplayState::Unload()
{
	if (mpCharacter != nullptr)
	{
		mpCharacter->Unload();
		delete mpCharacter;
		mpCharacter = nullptr;
	}
	mCursor.Unload();
	mMap.Unload();
}

GameState GameplayState::Update(float deltaTime)
{
	if (mpCharacter != nullptr)
	{
		mpCharacter->Update(deltaTime, mMap);

		// Keep screen centered on character by setting a screen based offset.
		Vector2 target = mpCharacter->GetPosition();
		const int kScreenWidth = IniFile_GetInt("WinWidth", 800);
		const int kScreenHeight = IniFile_GetInt("WinHeight", 600);
		const int kMapWidth = mMap.GetWidth();
		const int kMapHeight = mMap.GetHeight();

		Vector2 offset;
		offset.x = (kScreenWidth * 0.5f) - target.x;
		offset.x = Clamp(offset.x, (float)kScreenWidth - kMapWidth, 0.0f);
		offset.y = (kScreenHeight * 0.5f) - target.y;
		offset.y = Clamp(offset.y, (float)kScreenHeight - kMapHeight, 0.0f);

		mpCharacter->SetOffset(offset);
		mMap.SetOffset(offset);
	}

	mCursor.Update(deltaTime);

	if (Input_IsMousePressed(Mouse::LBUTTON))
	{
		const int screenX = Input_GetMouseScreenX();
		const int screenY = Input_GetMouseScreenY();
		const int type = mMap.GetTile(screenX, screenY);
		mMap.SetTile(screenX, screenY, type + 1);
	}

	GameState result = GameState::Invalid;
	if (Input_IsKeyPressed(Keys::ESCAPE))
	{
		result = GameState::Frontend;
	}

	return result;
}

void GameplayState::Render()
{
	mMap.Render();
	if (mpCharacter != nullptr)
	{
		mpCharacter->Render();
	}
	mCursor.Render();
}