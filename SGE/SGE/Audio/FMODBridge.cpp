//====================================================================================================
// Filename:	FMODBridge.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Audio/FMODBridge.h"

#include <assert.h>

#include "Core/Log.h"

//====================================================================================================
// Statics
//====================================================================================================

FMODBridge* FMODBridge::s_pInstance = 0;

//====================================================================================================
// Class Definitions
//====================================================================================================

void FMODBridge::Create()
{
	assert(s_pInstance == nullptr);
	s_pInstance = new FMODBridge();
}

//----------------------------------------------------------------------------------------------------

void FMODBridge::Destroy()
{
	assert(s_pInstance != nullptr);
	delete s_pInstance;
	s_pInstance = nullptr;
}

//----------------------------------------------------------------------------------------------------

FMODBridge* FMODBridge::Get()
{
	assert(s_pInstance != nullptr);
	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

FMODBridge::FMODBridge() :
	mpFMODSystem(0),
	mInitialized(false)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

FMODBridge::~FMODBridge()
{
	// Terminate the audio system
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void FMODBridge::Initialize(int iMaxChannels)
{
	// Check if we have already initialized the system
	if (mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[FMOD] System already initialized.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[FMOD] Initializing...");

	// Create FMOD system
	if (FMOD_OK != FMOD_System_Create(&mpFMODSystem))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[FMOD] Failed to create FMOD system.");
		return;
	}

	// Initialize FMOD system
	if (FMOD_OK != FMOD_System_Init(mpFMODSystem, iMaxChannels, FMOD_INIT_NORMAL, 0))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[FMOD] Failed to initialize FMOD system.");
		return;
	}

	// Set flag
	mInitialized = true;

	// Write to log
	Log::Get()->Write(LogType::Message, "[FMOD] System initialized.");
}

//----------------------------------------------------------------------------------------------------

void FMODBridge::Terminate()
{
	// Check if we have already terminated the manager
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[FMOD] System already terminated.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[FMOD] Terminating...");

	// Release FMOD
	if (0 != mpFMODSystem)
	{
		FMOD_System_Release(mpFMODSystem);
		mpFMODSystem = 0;
	}

	// Set flag
	mInitialized = false;

	// Write to log
	Log::Get()->Write(LogType::Message, "[FMOD] System terminated");
}

//----------------------------------------------------------------------------------------------------

void FMODBridge::Update()
{
	// Check if we have already initialized the system
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[FMOD] Failed to update audio. System not initialized.");
		return;
	}

	// Update FMOD
	FMOD_System_Update(mpFMODSystem);
}

//----------------------------------------------------------------------------------------------------

FMOD_SYSTEM* FMODBridge::FMODSystem() const
{
	// Check if we have already initialized the system
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[FMOD] Failed to access audio. System not initialized.");
		return 0;
	}

	return mpFMODSystem;
}