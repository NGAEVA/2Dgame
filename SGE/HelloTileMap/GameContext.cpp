
#include "GameContext.h"

GameContext::GameContext()
	: mLevel(0)
{
	// Empty
}

GameContext::~GameContext()
{
	// Empty
}

void GameContext::SaveData(const char* pFilename)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, pFilename, "w");
	if (pFile != nullptr)
	{
		// Write data
		fprintf_s(pFile, "Level: %d\n", mLevel);

		fclose(pFile);
	}
}

void GameContext::LoadData(const char* pFilename)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, pFilename, "r");
	if (pFile != nullptr)
	{
		// Read data
		fscanf_s(pFile, "%*s %d\n", &mLevel);

		fclose(pFile);
	}
}