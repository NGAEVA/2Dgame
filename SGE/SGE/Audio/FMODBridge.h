#ifndef INCLUDED_FMOD_H
#define INCLUDED_FMOD_H

//====================================================================================================
// Filename:	FMOD.h
// Description:	Singleton class for the audio system. This is a wrapper class for the FMOD sound API.
//			  The singleton can be accessed via a call to FMOD::Get().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "fmod.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class FMODBridge
{
public:
	// Functions to create/destroy the singleton
	static void Create();
	static void Destroy();

	// Accessor function for singleton instance
	static FMODBridge* Get();

	// Destructor
	virtual ~FMODBridge();

	// Functions to startup and shutdown the audio system
	void Initialize(int iMaxChannels);
	void Terminate();

	// Function to update the audio system
	void Update();

	// Function to access the FMOD sound system
	FMOD_SYSTEM* FMODSystem() const;

protected:
	// Protected constructor for singleton
	FMODBridge();

private:
	static FMODBridge* s_pInstance;	  // Static instance for singleton

	FMOD_SYSTEM* mpFMODSystem;	 // Pointer to FMOD system

	bool mInitialized;			 // Init flag
};

#endif // #ifndef INCLUDED_FMOD_H