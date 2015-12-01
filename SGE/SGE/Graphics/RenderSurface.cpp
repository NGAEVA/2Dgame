//====================================================================================================
// Filename:	RenderSurface.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "RenderSurface.h"

#include "Core/Log.h"
#include "Graphics/DXGraphics.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

RenderSurface::RenderSurface()
	: mpRenderTexture(nullptr)
	, mpRenderSurface(nullptr)
	, mpBackBuffer(nullptr)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

RenderSurface::~RenderSurface()
{
	// Release everything
	Destroy();
}

//----------------------------------------------------------------------------------------------------

void RenderSurface::Create(int iWidth, int iHeight)
{
	// Create a new render target
	HRESULT hr = DXGraphics::Get()->D3DDevice()->CreateTexture
	(
		iWidth,					// Texture width
		iHeight,				// Texture height
		1,						// Mipmap level
		D3DUSAGE_RENDERTARGET,	// Usage
		D3DFMT_A8R8G8B8,		// Format
		D3DPOOL_DEFAULT,		// Memory pool
		&mpRenderTexture,		// Pointer to texture interface
		nullptr
	);

	if (FAILED(hr))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[RenderSurface] Failed to create render surface.");
		return;
	}

	// Get the surface for the render target
	mpRenderTexture->GetSurfaceLevel(0, &mpRenderSurface);
}

//----------------------------------------------------------------------------------------------------

void RenderSurface::Destroy()
{
	// Release everything
	if (nullptr != mpBackBuffer)
	{
		mpBackBuffer->Release();
		mpBackBuffer = nullptr;
	}
	if (nullptr != mpRenderSurface)
	{
		mpRenderSurface->Release();
		mpRenderSurface = nullptr;
	}
	if (nullptr != mpRenderTexture)
	{
		mpRenderTexture->Release();
		mpRenderTexture = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------

void RenderSurface::Bind()
{
	// Retain current render surface
	DXGraphics::Get()->D3DDevice()->GetRenderTarget(0, &mpBackBuffer);

	// Set render target to texture
	DXGraphics::Get()->D3DDevice()->SetRenderTarget(0, mpRenderSurface);
}

//----------------------------------------------------------------------------------------------------

void RenderSurface::UnBind()
{
	// Restore original render surface
	DXGraphics::Get()->D3DDevice()->SetRenderTarget(0, mpBackBuffer);
	mpBackBuffer->Release();
}

//----------------------------------------------------------------------------------------------------

IDirect3DTexture9* RenderSurface::GetTexture() const
{
	return mpRenderTexture;
}