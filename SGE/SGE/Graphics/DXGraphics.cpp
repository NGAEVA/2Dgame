//====================================================================================================
// Filename:	DXGraphics.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics/DXGraphics.h"

#include <assert.h>

#include "Core/Log.h"

//====================================================================================================
// Statics
//====================================================================================================

DXGraphics* DXGraphics::s_pInstance = nullptr;

//====================================================================================================
// Class Definitions
//====================================================================================================

void DXGraphics::Create()
{
	assert(s_pInstance == nullptr);
	s_pInstance = new DXGraphics();
}

//----------------------------------------------------------------------------------------------------

void DXGraphics::Destroy()
{
	assert(s_pInstance != nullptr);
	delete s_pInstance;
	s_pInstance = nullptr;
}

//----------------------------------------------------------------------------------------------------

DXGraphics* DXGraphics::Get()
{
	assert(s_pInstance != nullptr);
	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

DXGraphics::DXGraphics()
	: mpD3D(nullptr)
	, mpD3DDevice(nullptr)
	, mInitialized(false)
{
	ZeroMemory(&mD3DDM, sizeof(mD3DDM));
	ZeroMemory(&mD3DPP, sizeof(mD3DPP));
}

//----------------------------------------------------------------------------------------------------

DXGraphics::~DXGraphics()
{
	// Terminate the graphics system
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void DXGraphics::Initialize(HWND hWindow, int iWidth, int iHeight, bool bFullscreen)
{
	// Check if we have already initialized the system
	if (mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[DXGraphics] System already initialized.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[DXGraphics] Initializing...");

	// Obtain an interface to Direct3D
	mpD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// Get device capabilities
	// http://msdn2.microsoft.com/en-us/library/bb172513.aspx
	D3DCAPS9 sD3DCaps;
	mpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,   // Get primary display adapter
						 D3DDEVTYPE_HAL,	   // Device type (HAL or REF)
						 &sD3DCaps);		  // Direct3D capability struct

	// HAL - Hardware Abstraction Layer (Hardware)
	// REF - Reference device (Software)

	// Check for hardware vertex processing
	int iVertexProcessing = 0;
	if (sD3DCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		iVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		iVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// Check for shader versions

	// Set presentation parameters
	// http://msdn2.microsoft.com/en-us/library/bb172588.aspx
	ZeroMemory(&mD3DPP, sizeof(mD3DPP));
	mD3DPP.BackBufferWidth				= iWidth;
	mD3DPP.BackBufferHeight				= iHeight;
	mD3DPP.BackBufferFormat				= D3DFMT_A8R8G8B8;
	mD3DPP.BackBufferCount				= 1;
	mD3DPP.MultiSampleType				= D3DMULTISAMPLE_NONE;
	mD3DPP.MultiSampleQuality			= 0;
	mD3DPP.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	mD3DPP.hDeviceWindow				= hWindow;
	mD3DPP.Windowed						= !bFullscreen;
	mD3DPP.EnableAutoDepthStencil		= true;
	mD3DPP.AutoDepthStencilFormat		= D3DFMT_D24S8;
	mD3DPP.Flags						= 0;
	mD3DPP.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	mD3DPP.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;

	// Create Direct3D device
	if (FAILED(mpD3D->CreateDevice(D3DADAPTER_DEFAULT,
								   D3DDEVTYPE_HAL,
								   hWindow,
								   iVertexProcessing,
								   &mD3DPP,
								   &mpD3DDevice)))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[DXGraphics] Failed to create Direct3D device.");
		return;
	}

	// Set flag
	mInitialized = true;

	// Write to log
	Log::Get()->Write(LogType::Message, "[DXGraphics] System initialized.");
}

//----------------------------------------------------------------------------------------------------

void DXGraphics::Terminate()
{
	// Check if we have already terminated the manager
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[DXGraphics] System already terminated.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[DXGraphics] Terminating...");

	// Release Direct3D device
	if (nullptr != mpD3DDevice)
	{
		mpD3DDevice->Release();
		mpD3DDevice = nullptr;
	}

	// Release Direct3D
	if (nullptr != mpD3D)
	{
		mpD3D->Release();
		mpD3D = nullptr;
	}

	// Set flag
	mInitialized = false;

	// Write to log
	Log::Get()->Write(LogType::Message, "[DXGraphics] System terminated");
}

//----------------------------------------------------------------------------------------------------

IDirect3DDevice9* DXGraphics::D3DDevice() const
{
	// Check if the system is initialized
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[DXGraphics] Trying to access graphics device without initializing.");
		return nullptr;
	}

	return mpD3DDevice;
}

//----------------------------------------------------------------------------------------------------

void DXGraphics::BeginRender(D3DCOLOR sClearColor)
{
	// Check if the system is initialized
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[DXGraphics] Graphics system not initialized.");
		return;
	}

	HRESULT hr = mpD3DDevice->TestCooperativeLevel();
	if (FAILED(hr))
	{
		// If device is lost, rendering is not possible and we will have to try again later
		if (hr == D3DERR_DEVICELOST)
		{
			Log::Get()->Write(LogType::Error, "[DXGraphics] Graphics device lost.");
			Sleep(100);
			return;
		}

		// If the device is ready again, then we can go ahead and reset it
		if (hr == D3DERR_DEVICENOTRESET)
		{
			Log::Get()->Write(LogType::Error, "[DXGraphics] Resetting graphics device.");

			// Release all device objects
			//InvalidateDeviceObjects();

			// Proceed and reset the device. Note that the reset will fail
			// unless the application releases all resources that are
			// allocated in D3DPOOL_DEFAULT, including any render targets
			// and buffers.
			hr = mpD3DDevice->Reset(&mD3DPP);
			if (FAILED(hr))
				return;

			// Restore all device objects
			//RestoreDeviceObjects();

			Log::Get()->Write(LogType::Error, "[DXGraphics] Graphics device reset succeeded.");
		}
	}

	// Clear the screen and the back buffer
	mpD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, sClearColor, 1.0f, 0);

	// Begin rendering
	mpD3DDevice->BeginScene();
}

//----------------------------------------------------------------------------------------------------

void DXGraphics::EndRender()
{
	// Check if the system is initialized
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[DXGraphics] Graphics system not initialized.");
		return;
	}

	// End rendering
	mpD3DDevice->EndScene();
}

//----------------------------------------------------------------------------------------------------

void DXGraphics::Present()
{
	// Check if the system is initialized
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[DXGraphics] Graphics system not initialized.");
		return;
	}

	// Present to display.
	mpD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}