//====================================================================================================
// Filename:	SpriteRenderer.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics/SpriteRenderer.h"

#include <assert.h>

#include "Core/Log.h"
#include "Graphics/Sprite.h"
#include "Graphics/DXGraphics.h"

//====================================================================================================
// Statics
//====================================================================================================

SpriteRenderer* SpriteRenderer::s_pInstance = nullptr;

//====================================================================================================
// Class Definitions
//====================================================================================================

void SpriteRenderer::Create()
{
	assert(s_pInstance == nullptr);
	s_pInstance = new SpriteRenderer();
}

//----------------------------------------------------------------------------------------------------

void SpriteRenderer::Destroy()
{
	assert(s_pInstance != nullptr);
	delete s_pInstance;
	s_pInstance = nullptr;
}

//----------------------------------------------------------------------------------------------------

SpriteRenderer* SpriteRenderer::Get()
{
	assert(s_pInstance != nullptr);
	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

SpriteRenderer::SpriteRenderer() :
	mpD3DXSprite(nullptr),
	mInitialized(false)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

SpriteRenderer::~SpriteRenderer()
{
	// Terminate the sprite renderer
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void SpriteRenderer::Initialize()
{
	// Check if we have already initialized the renderer
	if (mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[SpriteRenderer] Renderer already initialized.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[SpriteRenderer] Initializing...");

	// Create a D3DX sprite for rendering
	if (FAILED(D3DXCreateSprite(DXGraphics::Get()->D3DDevice(), &mpD3DXSprite)))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[SpriteRenderer] Failed to create D3DX sprite.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[SpriteRenderer] Renderer initialized.");

	// Set flag
	mInitialized = true;
}

//----------------------------------------------------------------------------------------------------

void SpriteRenderer::Terminate()
{
	// Check if we have already terminated the renderer
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[SpriteRenderer] Renderer already terminated.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[SpriteRenderer] Terminating...");

	// Release D3DX sprite
	if (nullptr != mpD3DXSprite)
	{
		mpD3DXSprite->Release();
		mpD3DXSprite = nullptr;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[SpriteRenderer] Renderer terminated.");

	// Set flag
	mInitialized = false;
}

//----------------------------------------------------------------------------------------------------

void SpriteRenderer::BeginRender()
{
	// Check if renderer is initialized
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[SpriteRenderer] Failed to render sprites. Renderer not initialized.");
		return;
	}

	// Start sprite rendering
	mpD3DXSprite->Begin(D3DXSPRITE_SORT_DEPTH_FRONTTOBACK | D3DXSPRITE_ALPHABLEND);
}

//----------------------------------------------------------------------------------------------------

void SpriteRenderer::EndRender()
{
	// Check if renderer is initialized
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[SpriteRenderer] Failed to render sprites. Renderer not initialized.");
		return;
	}

	// Commit sprite rendering
	mpD3DXSprite->End();
}

//----------------------------------------------------------------------------------------------------

ID3DXSprite* SpriteRenderer::D3DXSprite() const
{
	// Check if renderer is initialized
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[SpriteRenderer] Failed to get D3DX sprite. Renderer not initialized.");
		return nullptr;
	}

	return mpD3DXSprite;
}