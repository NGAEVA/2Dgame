#ifndef INCLUDED_TIMER_H
#define INCLUDED_TIMER_H

//====================================================================================================
// Filename:	Timer.h
// Description:	Singleton class for hi-res timer. It tracks the time elapsed between each Update()
//			  call and the total number of times Update() is called every second. The singleton can
//			  be accessed via a call to Timer::Get().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#define WIN32_LEAN_AND_MEAN	// Reduce windows include scope
#include <windows.h>

//====================================================================================================
// Class Declarations
//====================================================================================================

class Timer
{
public:
	// Functions to create/destroy the singleton
	static void Create();
	static void Destroy();

	// Accessor function for singleton instance
	static Timer* Get();

public:
	// Function to startup and shutdown the timer
	void Initialize();
	void Terminate();

	// Function to update the timer can calculate the new elapsed time
	float Update();

	// Function to get the elapsed time since last frame
	float GetElapsedSeconds() const;

	// Function to get the frame per second
	float GetFPS() const;

protected:
	// Protected constructor for singleton
	Timer();

private:
	static Timer* s_pInstance;		 // Static instance for singleton

	// http://msdn2.microsoft.com/en-us/library/aa383713.aspx
	LARGE_INTEGER   mTicksPerSecond; // System clock frequency
	LARGE_INTEGER   mLastTick;
	LARGE_INTEGER   mCurrentTick;

	float   mElapsedSeconds;		// Time passed since the last call to update
	float   mFPS;					// Calculated frame per second

	bool	mInitialized;			// Init flag
};

#endif // #ifndef INCLUDED_TIMER_H