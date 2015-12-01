#ifndef INCLUDED_SOUND_H
#define INCLUDED_SOUND_H

//====================================================================================================
// Filename:	Sound.h
// Description:	Class for a FMOD sound segment.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Resource/Resource.h"

#include "fmod.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class Sound : public Resource
{
public:
	// Constructor
	Sound();

	// Destructor
	virtual ~Sound();

	//==================================================
	// Resource Implementation
	//==================================================
	// Function to load/unload a new sound
	virtual void Load(const char* pFilename);
	virtual void Unload();
	//==================================================

	// Functions to control the playback of the sound
	void Play(bool bLoop = false);
	void Stop();
	void Pause();
	void Resume();
	bool IsPlaying() const;
	bool IsPaused() const;

	// Functions to get/set volume
	float GetVolume() const;
	void SetVolume(float fVolume);
	void AddToVolume(float fVolume);

private:
	FMOD_SOUND* mpSound;		// Pointer to a FMOD sound
	FMOD_CHANNEL* mpChannel;	// Pointer to a FMOD channel

	float mVolume;

	bool mLoaded;
};

#endif // #ifndef INCLUDED_SOUND_H