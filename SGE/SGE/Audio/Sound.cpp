//====================================================================================================
// Filename:	Sound.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Sound.h"

#include "Audio/FMODBridge.h"
#include "Core/Log.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

Sound::Sound() :
	mpSound(nullptr),
	mpChannel(nullptr),
	mVolume(1.0f),
	mLoaded(false)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

Sound::~Sound()
{
	// Release everything
	Unload();
}

//----------------------------------------------------------------------------------------------------

void Sound::Load(const char* pFilename)
{
	// Clear everything before we load the sound
	Unload();

	// Set mode
	FMOD_MODE mode = FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE;

	// Load the sound using FMOD system
	if (FMOD_OK != FMOD_System_CreateSound(FMODBridge::Get()->FMODSystem(), pFilename, mode, 0, &mpSound))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Sound] Failed to load sound %s", pFilename);
		return;
	}

	// Set flag
	mLoaded = true;
}

//----------------------------------------------------------------------------------------------------

void Sound::Unload()
{
	// Stop playing first
	Stop();

	// Release everything
	if (0 != mpSound)
	{
		FMOD_Sound_Release(mpSound);
		mpSound = 0;
	}

	// Reset flag
	mLoaded = false;
}

//----------------------------------------------------------------------------------------------------

void Sound::Play(bool bLoop)
{
	// Check if we have a sound loaded
	if (!mLoaded)
	{
		return;
	}

	// Stop any previous sound first
	Stop();

	// Check if we are already playing
	FMOD_System_PlaySound(FMODBridge::Get()->FMODSystem(), FMOD_CHANNEL_FREE, mpSound, true, &mpChannel);
	FMOD_Channel_SetLoopCount(mpChannel, bLoop ? -1 : 0);
	FMOD_Channel_SetVolume(mpChannel, mVolume);
	FMOD_Channel_SetPaused(mpChannel, false);
}

//----------------------------------------------------------------------------------------------------

void Sound::Stop()
{
	// Check if we have a sound loaded
	if (!mLoaded)
	{
		return;
	}

	// Check if we have a valid channel
	if (0 != mpChannel)
	{
		// Stop the channel that is playing the sound
		FMOD_Channel_Stop(mpChannel);
		mpChannel = 0;
	}
}

//----------------------------------------------------------------------------------------------------

void Sound::Pause()
{
	// Check if we have a sound loaded
	if (!mLoaded)
	{
		return;
	}

	// Check if we have a valid channel
	if (0 != mpChannel)
	{
		// Pause the channel that is playing the sound
		FMOD_Channel_SetPaused(mpChannel, 1);		
	}
}

//----------------------------------------------------------------------------------------------------

void Sound::Resume()
{
	// Check if we have a sound loaded
	if (!mLoaded)
	{
		return;
	}

	// Check if we have a valid channel
	if (0 != mpChannel)
	{
		// Resume the channel that is playing the sound
		FMOD_Channel_SetPaused(mpChannel, 0);		
	}
}

//----------------------------------------------------------------------------------------------------

bool Sound::IsPlaying() const
{
	// Check if we have a sound loaded
	if (!mLoaded)
	{
		return false;
	}

	// Check if we have a valid channel
	FMOD_BOOL bPlaying = 0;
	if (0 != mpChannel)
	{
		FMOD_Channel_IsPlaying(mpChannel, &bPlaying);
	}

	return (bPlaying != 0);
}

//----------------------------------------------------------------------------------------------------

bool Sound::IsPaused() const
{
	// Check if we have a sound loaded
	if (!mLoaded)
	{
		return false;
	}

	// Check if we have a valid channel
	FMOD_BOOL bPaused = 0;
	if (0 != mpChannel)
	{
		FMOD_Channel_GetPaused(mpChannel, &bPaused);
	}

	return (bPaused != 0);
}

//----------------------------------------------------------------------------------------------------

float Sound::GetVolume() const
{
	return mVolume;
}

//----------------------------------------------------------------------------------------------------

void Sound::SetVolume(float fVolume)
{
	// Check if we have a sound loaded
	if (!mLoaded)
	{
		return;
	}

	// Cache the volume
	mVolume = fVolume;

	if (mVolume> 1.0f) mVolume = 1.0f;
	else if (mVolume <0.0f) mVolume = 0.0f;

	// Check if we have a valid channel
	if (0 != mpChannel)
	{
		FMOD_Channel_SetVolume(mpChannel, mVolume);
	}
}

//----------------------------------------------------------------------------------------------------

void Sound::AddToVolume(float fVolume)
{
	// Check if we have a sound loaded
	if (!mLoaded)
	{
		return;
	}

	// Cache the volume
	mVolume += fVolume;

	if (mVolume> 1.0f) mVolume = 1.0f;
	else if (mVolume <0.0f) mVolume = 0.0f;

	// Check if we have a valid channel
	if (0 != mpChannel)
	{
		FMOD_Channel_SetVolume(mpChannel, mVolume);
	}
}