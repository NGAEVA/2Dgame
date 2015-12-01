//====================================================================================================
// Filename:	DXInput.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Input/DXInput.h"

#include <assert.h>

#include "Core/Log.h"

//====================================================================================================
// Statics
//====================================================================================================

DXInput* DXInput::s_pInstance = nullptr;

//====================================================================================================
// Callback Function
//====================================================================================================

BOOL CALLBACK EnumGamePadCallback(const DIDEVICEINSTANCE* pDIDeviceInstance, VOID* pContext)
{
	// Obtain an interface to the enumerated joystick.
	IDirectInput8*		  pDI = DXInput::Get()->mpDI;
	IDirectInputDevice8**   pGamePad = &(DXInput::Get()->mpGamePad);
	if (FAILED(pDI->CreateDevice(pDIDeviceInstance->guidInstance, pGamePad, nullptr))) 
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[Input] Failed to create game pad device.");
	}

	return DIENUM_STOP;
}

//====================================================================================================
// Class Definitions
//====================================================================================================

void DXInput::Create()
{
	assert(s_pInstance == nullptr);
	s_pInstance = new DXInput();
}

//----------------------------------------------------------------------------------------------------

void DXInput::Destroy()
{
	assert(s_pInstance != nullptr);
	delete s_pInstance;
	s_pInstance = nullptr;
}

//----------------------------------------------------------------------------------------------------

DXInput* DXInput::Get()
{
	assert(s_pInstance != nullptr);
	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

DXInput::DXInput()
	: mpDI(nullptr)
	, mpKeyboard(nullptr)
	, mpMouse(nullptr)
	, mpGamePad(nullptr)
	, mMouseX(0)
	, mMouseY(0)
	, mScreenWidth(0)
	, mScreenHeight(0)
	, mInitialized(false)
{
	ZeroMemory(mCurrKeyBuffer, kKeyBufferSize);
	ZeroMemory(mPrevKeyBuffer, kKeyBufferSize);
	ZeroMemory(&mCurrMouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&mPrevMouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&mCurrGamePadState, sizeof(DIJOYSTATE));
	ZeroMemory(&mPrevGamePadState, sizeof(DIJOYSTATE));
}

//----------------------------------------------------------------------------------------------------

void DXInput::Initialize(HWND hWindow)
{
	// Check if we have already initialized the system
	if (mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[Input] System already initialized.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[Input] Initializing...");

	// Obtain an interface to DirectInput
	if (FAILED(DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mpDI, nullptr)))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Input] Failed to create DirectInput object.");
		return;
	}

	//----------------------------------------------------------------------------------------------------
	// Create keyboard device
	if (FAILED(mpDI->CreateDevice(GUID_SysKeyboard, &mpKeyboard, nullptr)))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Input] Failed to create keyboard device.");
		return;
	}

	// Set the keyboard data format
	if (FAILED(mpKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Input] Failed to set keyboard data format.");
		return;
	}

	// Set the keyboard cooperative level
	if (FAILED(mpKeyboard->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY)))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Input] Failed to set keyboard cooperative level.");
		return;
	}

	// Acquire the keyboard device
	if (FAILED(mpKeyboard->Acquire()))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Input] Failed to acquire keyboard device.");
	}
	//----------------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------------
	// Create mouse device
	if (FAILED(mpDI->CreateDevice(GUID_SysMouse, &mpMouse, nullptr)))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Input] Failed to create mouse device.");
		return;
	}

	// Set the mouse data format
	if (FAILED(mpMouse->SetDataFormat(&c_dfDIMouse)))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Input] Failed to set mouse data format.");
		return;
	}

	// Set the mouse cooperative level
	if (FAILED(mpMouse->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Input] Failed to set mouse cooperative level.");
		return;
	}

	// Acquire the mouse device
	if (FAILED(mpMouse->Acquire()))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Input] Failed to acquire mouse device.");
	}

	// Calculate starting mouse position
	RECT clientRect;
	GetClientRect(hWindow, &clientRect);
	GetWindowRect(hWindow, &clientRect);
	mScreenWidth = clientRect.right - clientRect.left;
	mScreenHeight = clientRect.bottom - clientRect.top;
	mMouseX = mScreenWidth>> 1;
	mMouseY = mScreenHeight>> 1;
	//----------------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------------
	// Enumerate for game pad device
	if (FAILED(mpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumGamePadCallback, nullptr, DIEDFL_ATTACHEDONLY)))
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[Input] Failed to enumerate for game pad devices.");
	}

	// Check if we have a game pad detected
	if (nullptr != mpGamePad)
	{
		// Set the game pad data format
		if (FAILED(mpGamePad->SetDataFormat(&c_dfDIJoystick)))
		{
			// Write to log
			Log::Get()->Write(LogType::Error, "[Input] Failed to set game pad data format.");
			return;
		}

		// Set the game pad cooperative level
		if (FAILED(mpGamePad->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			// Write to log
			Log::Get()->Write(LogType::Error, "[Input] Failed to set game pad cooperative level.");
			return;
		}

		// Acquire the game pad device
		if (FAILED(mpGamePad->Acquire()))
		{
			// Write to log
			Log::Get()->Write(LogType::Error, "[Input] Failed to acquire game pad device.");
		}
	}
	else
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[Input] No game pad attached.");
	}

	// Set flag
	mInitialized = true;

	// Write to log
	Log::Get()->Write(LogType::Message, "[Input] System initialized.");
}

//----------------------------------------------------------------------------------------------------

void DXInput::Terminate()
{
	// Check if we have already terminated the system
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[Input] System already terminated.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[Input] Terminating...");

	// Release devices
	if (nullptr != mpGamePad)
	{
		mpGamePad->Unacquire();
		mpGamePad->Release();
		mpGamePad = nullptr;
	}
	if (nullptr != mpMouse)
	{
		mpMouse->Unacquire();
		mpMouse->Release();
		mpMouse = nullptr;
	}
	if (nullptr != mpKeyboard)
	{
		mpKeyboard->Unacquire();
		mpKeyboard->Release();
		mpKeyboard = nullptr;
	}

	// Release DirectInput interface
	if (nullptr != mpDI)
	{
		mpDI->Release();
		mpDI = nullptr;
	}

	// Set flag
	mInitialized = false;

	// Write to log
	Log::Get()->Write(LogType::Message, "[Input] System terminated");
}

//----------------------------------------------------------------------------------------------------

void DXInput::Update()
{
	// Check if the system is initialized
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Input] Failed to update input devices. System not initialized.");
		return;
	}

	// Update keyboard
	if (nullptr != mpKeyboard)
	{
		UpdateKeyboard();
	}

	// Update mouse
	if (nullptr != mpMouse)
	{
		UpdateMouse();
	}

	// Update game pad
	if (nullptr != mpGamePad)
	{
		UpdateGamePad();
	}
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsKeyDown(int iKey) const
{
	return (mCurrKeyBuffer[iKey] & 0x80) ? true : false;
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsKeyPressed(int iKey) const
{
	bool bCurrState = (mCurrKeyBuffer[iKey] & 0x80) ? true : false;
	bool bPrevState = (mPrevKeyBuffer[iKey] & 0x80) ? true : false;

	return !bPrevState && bCurrState;
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsMouseDown(int iMouse) const
{
	return (mCurrMouseState.rgbButtons[iMouse] & 0x80) ? true : false;
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsMousePressed(int iMouse) const
{
	bool bCurrState = (mCurrMouseState.rgbButtons[iMouse] & 0x80) ? true : false;
	bool bPrevState = (mPrevMouseState.rgbButtons[iMouse] & 0x80) ? true : false;

	return !bPrevState && bCurrState;
}

//----------------------------------------------------------------------------------------------------

int DXInput::GetMouseScreenX() const
{
	return mMouseX;
}

//----------------------------------------------------------------------------------------------------

int DXInput::GetMouseScreenY() const
{
	return mMouseY;
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsMouseLeftEdge() const
{
	return (mMouseX == 0);
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsMouseRightEdge() const
{
	return (mMouseX == mScreenWidth - 1);
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsMouseTopEdge() const
{
	return (mMouseY == 0);
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsMouseBottomEdge() const
{
	return (mMouseY == mScreenHeight - 1);
}

//----------------------------------------------------------------------------------------------------

int DXInput::GetMouseMoveX() const
{
	return mCurrMouseState.lX;
}

//----------------------------------------------------------------------------------------------------

int DXInput::GetMouseMoveY() const
{
	return mCurrMouseState.lY;
}

//----------------------------------------------------------------------------------------------------

int DXInput::GetMouseMoveZ() const
{
	return mCurrMouseState.lZ;
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsGamePadButtonDown(int iButton) const
{
	if (nullptr == mpGamePad)
	{
		return false;
	}

	return (mCurrGamePadState.rgbButtons[iButton] & 0x80) ? true : false;
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsGamePadButtonPressed(int iButton) const
{
	if (nullptr == mpGamePad)
	{
		return false;
	}

	bool bCurrState = (mCurrGamePadState.rgbButtons[iButton] & 0x80) ? true : false;
	bool bPrevState = (mPrevGamePadState.rgbButtons[iButton] & 0x80) ? true : false;

	return !bPrevState && bCurrState;
}
//----------------------------------------------------------------------------------------------------

bool DXInput::IsDPadUp() const
{
	if (nullptr == mpGamePad)
	{
		return false;
	}

	return (mCurrGamePadState.rgdwPOV[0] == 0);
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsDPadDown() const
{
	if (nullptr == mpGamePad)
	{
		return false;
	}

	return (mCurrGamePadState.rgdwPOV[0] == 18000);
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsDPadLeft() const
{
	if (nullptr == mpGamePad)
	{
		return false;
	}

	return (mCurrGamePadState.rgdwPOV[0] == 27000);
}

//----------------------------------------------------------------------------------------------------

bool DXInput::IsDPadRight() const
{
	if (nullptr == mpGamePad)
	{
		return false;
	}

	return (mCurrGamePadState.rgdwPOV[0] == 9000);
}

//----------------------------------------------------------------------------------------------------

float DXInput::GetLeftAnalogX() const
{
	if (nullptr == mpGamePad)
	{
		return 0.0f;
	}

	return (mCurrGamePadState.lX / 32767.5f) - 1.0f;
}

//----------------------------------------------------------------------------------------------------

float DXInput::GetLeftAnalogY() const
{
	if (nullptr == mpGamePad)
	{
		return 0.0f;
	}

	return -(mCurrGamePadState.lY / 32767.5f) + 1.0f;
}

//----------------------------------------------------------------------------------------------------

float DXInput::GetRightAnalogX() const
{
	if (nullptr == mpGamePad)
	{
		return 0.0f;
	}

	return (mCurrGamePadState.lZ / 32767.5f) - 1.0f;
}

//----------------------------------------------------------------------------------------------------

float DXInput::GetRightAnalogY() const
{
	if (nullptr == mpGamePad)
	{
		return 0.0f;
	}

	return -(mCurrGamePadState.lRz / 32767.5f) + 1.0f;
}

//----------------------------------------------------------------------------------------------------

void DXInput::UpdateKeyboard()
{
	// Store the previous keyboard state
	memcpy(mPrevKeyBuffer, mCurrKeyBuffer, kKeyBufferSize);

	// Get keyboard state
	static bool s_WriteToLog = true;
	HRESULT hr = mpKeyboard->GetDeviceState(kKeyBufferSize, (void*)mCurrKeyBuffer);
	if (FAILED(hr))
	{   
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			// Write to log
			if (s_WriteToLog)
			{
				Log::Get()->Write(LogType::Warning, "[Input] Keyboard device is lost.");
				s_WriteToLog = false;
			}

			// Try to acquire keyboard device again
			mpKeyboard->Acquire();
		}
		else
		{
			// Write to log
			Log::Get()->Write(LogType::Error, "[Input] Failed to get keyboard state.");
			return;
		}
	}
	else
	{
		// Reset flag
		s_WriteToLog = true;
	}
}

//----------------------------------------------------------------------------------------------------

void DXInput::UpdateMouse()
{
	// Store the previous mouse state
	memcpy(&mPrevMouseState, &mCurrMouseState, sizeof(DIMOUSESTATE));

	// Get mouse state
	static bool s_WriteToLog = true;
	HRESULT hr = mpMouse->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&mCurrMouseState);
	if (FAILED(hr))
	{   
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			// Write to log
			if (s_WriteToLog)
			{
				Log::Get()->Write(LogType::Warning, "[Input] Mouse device is lost.");
				s_WriteToLog = false;
			}

			// Try to acquire mouse device again
			mpMouse->Acquire();
		}
		else
		{
			// Write to log
			Log::Get()->Write(LogType::Error, "[Input] Failed to get mouse state.");
			return;
		}
	}
	else
	{
		// Reset flag
		s_WriteToLog = true;

		// Update mouse position
		mMouseX += mCurrMouseState.lX;
		mMouseY += mCurrMouseState.lY;

		// Check bound
		if (mMouseX <0)
			mMouseX = 0;
		if (mMouseX>= mScreenWidth)
			mMouseX = mScreenWidth - 1;
		if (mMouseY <0)
			mMouseY = 0;
		if (mMouseY>= mScreenHeight)
			mMouseY = mScreenHeight - 1;
	}
}

//----------------------------------------------------------------------------------------------------

void DXInput::UpdateGamePad()
{
	// Store the previous game pad state
	memcpy(&mPrevGamePadState, &mCurrGamePadState, sizeof(DIJOYSTATE));

	// Poll the game pad device
	static bool s_WriteToLog = true;
	HRESULT hr = mpGamePad->Poll();
	if (FAILED(hr))
	{   
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			// Write to log
			if (s_WriteToLog)
			{
				Log::Get()->Write(LogType::Warning, "[Input] Game pad device is lost.");
				s_WriteToLog = false;
			}

			// Try to acquire game pad device again
			mpGamePad->Acquire();
		}
		else
		{
			// Write to log
			Log::Get()->Write(LogType::Error, "[Input] Failed to get game pad state.");
			return;
		}
	}
	else
	{
		// Reset flag
		s_WriteToLog = true;
	}

	// Get game pad state
	hr = mpGamePad->GetDeviceState(sizeof(DIJOYSTATE), (void*)&mCurrGamePadState);
	if (FAILED(hr))
	{   
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			// Write to log
			if (s_WriteToLog)
			{
				Log::Get()->Write(LogType::Warning, "[Input] Game pad device is lost.");
				s_WriteToLog = false;
			}

			// Try to acquire game pad device again
			mpGamePad->Acquire();
		}
		else
		{
			// Write to log
			Log::Get()->Write(LogType::Error, "[Input] Failed to get game pad state.");
			return;
		}
	}
	else
	{
		// Reset flag
		s_WriteToLog = true;
	}
}