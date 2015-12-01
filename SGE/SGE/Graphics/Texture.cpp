//====================================================================================================
// Filename:	Texture.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Texture.h"

#include "Core/Log.h"
#include "Graphics/DXGraphics.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

Texture::Texture() :
	mpTexture(nullptr),
	mWidth(0),
	mHeight(0)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

Texture::~Texture()
{
	// Release everything
	Unload();
}

//----------------------------------------------------------------------------------------------------

void Texture::Load(const char* pFilename)
{
	// Clear everything before we create the texture
	Unload();

	// Struct for getting image info
	D3DXIMAGE_INFO info;

	// Load the texture from file
	HRESULT hr = D3DXCreateTextureFromFileExA
	(
		DXGraphics::Get()->D3DDevice(),	// Pointer to Direct 3D device interface
		pFilename,						  // Image filename
		D3DX_DEFAULT_NONPOW2,			   // Texture width (D3DX_DEFAULT_NONPOW2 = use texture size/no rounding)
		D3DX_DEFAULT_NONPOW2,			   // Texture height (D3DX_DEFAULT_NONPOW2 = use texture size/no rounding)
		0,								  // Mip levels (0 = full chain)
		D3DUSAGE_DYNAMIC,				   // Usage,
		D3DFMT_UNKNOWN,					 // Pixel format (D3DFMT_UNKNOWN = read from file)
		D3DPOOL_DEFAULT,					// Memory pool (D3DFMT_UNKNOWN = let D3D decide)
		D3DX_FILTER_NONE,				   // Filter
		D3DX_DEFAULT,					   // Mip-map filter
		0,								  // Color key for transparency (0 = disable)
		&info,							  // Texture info
		nullptr,							   // Palette entries
		&mpTexture						 // Pointer to Direct 3D texture interface
	);

	if (FAILED(hr))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Texture] Failed to create texture from file %s", pFilename);
		return;
	}

	// Cache texture dimensions
	mWidth = info.Width;
	mHeight = info.Height;
}

//----------------------------------------------------------------------------------------------------

void Texture::Unload()
{
	// Release everything
	if (nullptr != mpTexture)
	{
		mpTexture->Release();
		mpTexture = nullptr;
	}

	// Reset parameters
	mWidth = 0;
	mHeight = 0;
}

//----------------------------------------------------------------------------------------------------

IDirect3DTexture9* Texture::GetTexture() const
{
	return mpTexture;
}

//----------------------------------------------------------------------------------------------------

int Texture::GetWidth() const
{
	return mWidth;
}

//----------------------------------------------------------------------------------------------------

int Texture::GetHeight() const
{
	return mHeight;
}