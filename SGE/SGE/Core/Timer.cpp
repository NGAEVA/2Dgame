//====================================================================================================
// Filename:	Timer.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Core/Timer.h"

#include <assert.h>

#include "Core/Log.h"

//====================================================================================================
// Statics
//====================================================================================================

Timer* Timer::s_pInstance = nullptr;

//====================================================================================================
// Class Definitions
//====================================================================================================

void Timer::Create()
{
	assert(s_pInstance == nullptr);
	s_pInstance = new Timer();
}

//----------------------------------------------------------------------------------------------------

void Timer::Destroy()
{
	assert(s_pInstance != nullptr);
	delete s_pInstance;
	s_pInstance = nullptr;
}

//----------------------------------------------------------------------------------------------------

Timer* Timer::Get()
{
	assert(s_pInstance != nullptr);
	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

Timer::Timer()
	: mElapsedSeconds(0.0f)
	, mFPS(0.0f)
	, mInitialized(false)
{
	mTicksPerSecond.QuadPart = 0;
	mLastTick.QuadPart = 0;
	mCurrentTick.QuadPart = 0;
}

//----------------------------------------------------------------------------------------------------

void Timer::Initialize()
{
	// Check if we have already initialized the timer
	if (mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[Timer] Timer already initialized.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[Timer] Initializing...");

	// Get the system clock frequency, for time calculation later
	if (!QueryPerformanceFrequency(&mTicksPerSecond))
	{
		Log::Get()->Write(LogType::Error, "[Timer] Hi-res timer not supported.");
		return;
	}

	// Get the current tick count
	QueryPerformanceCounter(&mCurrentTick);
	mLastTick = mCurrentTick;

	// Set flag
	mInitialized = true;

	// Write to log
	Log::Get()->Write(LogType::Message, "[Timer] Timer initialized.");
}

//----------------------------------------------------------------------------------------------------

void Timer::Terminate()
{
	// Check if we have already terminated the timer
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[Timer] Timer already terminated.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[Timer] Terminating...");

	// Reset values
	mTicksPerSecond.QuadPart = 0;
	mLastTick.QuadPart = 0;
	mCurrentTick.QuadPart = 0;
	mElapsedSeconds = 0.0f;
	mFPS = 0.0f;

	// Set flag
	mInitialized = false;

	// Write to log
	Log::Get()->Write(LogType::Message, "[Timer] Timer terminated.");
}

//----------------------------------------------------------------------------------------------------

float Timer::Update()
{
	// Make sure the timer is initialized
	if (!mInitialized)
	{
		Log::Get()->Write(LogType::Error, "[Timer] Timer not initialized.");
		return 0.0f;
	}

	// Get the current tick count
	QueryPerformanceCounter(&mCurrentTick);

	// Calculate the elapsed time
	mElapsedSeconds =  static_cast<float>(mCurrentTick.QuadPart - mLastTick.QuadPart) / mTicksPerSecond.QuadPart;

	// Update the last tick count
	mLastTick = mCurrentTick;

	static float s_FrameSinceLastSecond = 0.0f;
	static float s_AccumulatedTime = 0.0f;

	// Calculate the FPS
	s_AccumulatedTime += mElapsedSeconds;
	s_FrameSinceLastSecond += 1.0f;

	// Update FPS is one second has passed
	if (s_AccumulatedTime>= 1.0f)
	{
		mFPS = s_FrameSinceLastSecond / s_AccumulatedTime;
		s_AccumulatedTime = 0.0f;
		s_FrameSinceLastSecond = 0.0f;
	}

	return mElapsedSeconds;
}

//----------------------------------------------------------------------------------------------------

float Timer::GetElapsedSeconds() const
{
	return mElapsedSeconds;
}

//----------------------------------------------------------------------------------------------------

float Timer::GetFPS() const
{
	return mFPS;
}