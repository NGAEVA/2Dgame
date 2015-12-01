//====================================================================================================
// Filename:	SGE.cpp
//====================================================================================================

//====================================================================================================
// Libraries
//====================================================================================================

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "../SGE/External/Lib/FMOD/fmodex_vc.lib")
#pragma comment (lib, "winmm.lib")

//====================================================================================================
// Includes
//====================================================================================================

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Audio/AudioSystem.h"
#include "Core/CoreSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Input/InputSystem.h"
#include "Misc/Misc.h"
#include "Resource/ResourceSystem.h"
#include "Utility/UtilitySystem.h"

#include "SGE.h"

//====================================================================================================
// Externs
//====================================================================================================

extern Game* SGE_CreateGame();

//====================================================================================================
// Main
//====================================================================================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Load init file
	IniFile::Create();
	IniFile::Get()->Load("sge.ini");

	// Check if we are showing the splash screen
	const bool bSplashScreen = IniFile::Get()->GetBool("SplashScreen", false);
	if (bSplashScreen)
	{
		// Splash texture name
		std::string fullname;
		fullname += IniFile::Get()->GetString("TexturePath", "../Data/Textures/");
		fullname += "splashscreen.bmp";

		// Splash screen
		SplashScreen splash;
		splash.Initialize(fullname.c_str());
		splash.Show(2000);
		splash.Terminate();
	}

	// Get initialization parameters
	const char* pAppName = IniFile::Get()->GetString("AppName", "SGE");
	const int iWinWidth = IniFile::Get()->GetInt("WinWidth", 800);
	const int iWinHeight = IniFile::Get()->GetInt("WinHeight", 600);

	
	// Initialize and run the game
	Game* game = SGE_CreateGame();
	game->Initialize(pAppName, hInstance, iWinWidth, iWinHeight);
	game->Run();
	game->Terminate();
	delete game;

	// Unload init file
	IniFile::Get()->Unload();
	IniFile::Destroy();

	// Dump any memory leaks
	_CrtDumpMemoryLeaks();

	return 0;
}


//====================================================================================================
// Class Definitions
//====================================================================================================

Game::Game()
	: mDebug(IniFile::Get()->GetBool("Debug", false))
	, mFPS(IniFile::Get()->GetBool("FPS", false))
	, mPostProcessing(IniFile::Get()->GetBool("PostProcessing", false))
	, mDebugStepFrame(false)
	, mPaused(false)
{
	mFont = new Font();
}

//----------------------------------------------------------------------------------------------------

Game::~Game()
{
	delete mFont;
	mFont = nullptr;
}

//----------------------------------------------------------------------------------------------------

void Game::OnInitialize()
{
	const int iWinWidth = IniFile::Get()->GetInt("WinWidth", 800);
	const int iWinHeight = IniFile::Get()->GetInt("WinHeight", 600);
	const int iResourceLimit = IniFile::Get()->GetInt("ResourceLimit", 10000);
	const bool bFullscreen = IniFile::Get()->GetBool("Fullscreen", false);

	// Initialize singletons
	Timer::Create();
	DXGraphics::Create();
	SpriteRenderer::Create();
	PostProcess::Create();
	DXInput::Create();
	ResourceManager::Create();
	FMODBridge::Create();

	// Initialize all engine systems
	Random::SetRandSeed();
	Timer::Get()->Initialize();
	DXGraphics::Get()->Initialize(mWindow, iWinWidth, iWinHeight, bFullscreen);
	SpriteRenderer::Get()->Initialize();
	DXInput::Get()->Initialize(mWindow);
	ResourceManager::Get()->Initialize(iResourceLimit);
	FMODBridge::Get()->Initialize(100);

	// Debug initialization
	if (mDebug)
	{
		Debug::Initialize();
	}

	// Post processing initialization
	if (mPostProcessing)
	{
		PostProcess::Get()->Initialize(iWinWidth, iWinHeight, "postprocess.fx");
	}

	// Create a new font
	mFont->Create(FontType::ARIAL);
	mFont->SetColor(255, 255, 0);

	// User init logic
	OnInitializeImpl();
}

//----------------------------------------------------------------------------------------------------

void Game::OnTerminate()
{
	// User clean up logic
	OnTerminateImpl();

	// Destroy the font
	mFont->Destroy();
		
	// Post processing termination
	if (mPostProcessing)
	{
		PostProcess::Get()->Terminate();
	}

	// Debug termination
	if (mDebug)
	{
		Debug::Terminate();
	}

	// Shutdown all engine systems
	FMODBridge::Get()->Terminate();
	ResourceManager::Get()->Terminate();
	DXInput::Get()->Terminate();
	SpriteRenderer::Get()->Terminate();
	DXGraphics::Get()->Terminate();
	Timer::Get()->Terminate();

	// Destroy all singletons
	FMODBridge::Destroy();
	ResourceManager::Destroy();
	DXInput::Destroy();
	PostProcess::Destroy();
	SpriteRenderer::Destroy();
	DXGraphics::Destroy();
	Timer::Destroy();
}

//----------------------------------------------------------------------------------------------------

void Game::OnMainLoop()
{
	// Get the new delta time (time passed since last frame).
	float dt = Timer::Get()->Update();

	// Update input.
	DXInput::Get()->Update();

	HandlePause();
	UpdatePass(dt);
	RenderPass(dt);
}

//----------------------------------------------------------------------------------------------------

void Game::HandlePause()
{
	if (mDebug == true)
	{
		if (SGE::Input_IsKeyPressed(Keys::TAB) == true)
		{
			mPaused = !mPaused;
		}

		mDebugStepFrame = false;
		
		if (mPaused == true && SGE::Input_IsKeyPressed(Keys::ENTER) == true)
		{
			 mDebugStepFrame = true;
		}
	}
}

//----------------------------------------------------------------------------------------------------

void Game::UpdatePass(float dt)
{
	if (mPaused == false || (mPaused == true && mDebugStepFrame == true))
	{
		if (mDebug == true)
 		{
			Debug::BeginRender();
		}

		// Update sound
		FMODBridge::Get()->Update();

		// User update logic
		if (OnUpdateImpl(dt))
		{
			RequestQuit();
		}
	}
}

//----------------------------------------------------------------------------------------------------

void Game::RenderPass(float dt)
{
	// Begin rendering
	PostProcess::Get()->BeginRender();
	DXGraphics::Get()->BeginRender();
	SpriteRenderer::Get()->BeginRender();

	// User render logic
	OnRenderImpl(dt);

	// Render FPS
	if (mFPS)
	{
		char temp[256];
		sprintf_s(temp, "FPS: %f", Timer::Get()->GetFPS());
		mFont->PrintText(temp, 0, 0);
	}

	if (mPaused)
	{
		mFont->PrintText("Paused", 0, 25);
	}

	// End rendering
	SpriteRenderer::Get()->EndRender();
	DXGraphics::Get()->EndRender();
	PostProcess::Get()->EndRender();

	if (mDebug == true)
 	{
		Debug::EndRender();
	}

	// Finalize
	DXGraphics::Get()->Present();
}


//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// SGE Functions
//====================================================================================================

void LogMessage(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	Log::Get()->Write(LogType::Message, msg, args);
	va_end(args);
}

//----------------------------------------------------------------------------------------------------

void LogWarning(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	Log::Get()->Write(LogType::Warning, msg, args);
	va_end(args);
}

//----------------------------------------------------------------------------------------------------

void LogError(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	Log::Get()->Write(LogType::Error, msg, args);
	va_end(args);
}

//----------------------------------------------------------------------------------------------------

int IniFile_GetInt(const char* pTag, int iDefault)
{
	return IniFile::Get()->GetInt(pTag, iDefault);
}

//----------------------------------------------------------------------------------------------------

bool IniFile_GetBool(const char* pTag, bool bDefault)
{
	return IniFile::Get()->GetBool(pTag, bDefault);
}

//----------------------------------------------------------------------------------------------------

float IniFile_GetFloat(const char* pTag, float fDefault)
{
	return IniFile::Get()->GetFloat(pTag, fDefault);
}

//----------------------------------------------------------------------------------------------------

const char* IniFile_GetString(const char* pTag, const char* pDefault)
{
	return IniFile::Get()->GetString(pTag, pDefault);
}

//----------------------------------------------------------------------------------------------------

int Random_GetInt(int iLower, int iUpper)
{
	return Random::GetInt(iLower, iUpper);
}

//----------------------------------------------------------------------------------------------------

float Random_GetFloat(float fLower, float fUpper)
{
	return Random::GetFloat(fLower, fUpper);
}

//----------------------------------------------------------------------------------------------------

Vector2 Random_GetVector2(const Vector2& vLower, const Vector2& vUpper)
{
	D3DXVECTOR2 lower(vLower.x, vLower.y);
	D3DXVECTOR2 upper(vUpper.x, vUpper.y);
	D3DXVECTOR2 vec(Random::GetVector2(lower, upper));

	return Vector2(vec.x, vec.y);
}

} // namespace SGE