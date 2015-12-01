#ifndef INCLUDED_TEXTURE_H
#define INCLUDED_TEXTURE_H

//====================================================================================================
// Filename:	Texture.h
// Description:	Class for a Direct3D texture.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Resource/Resource.h"

#include <d3dx9.h>

//====================================================================================================
// Class Declarations
//====================================================================================================

class Texture : public Resource
{
public:
	// Constructor
	Texture();

	// Destructor
	virtual ~Texture();

	//==================================================
	// Resource Implementation
	//==================================================
	// Function to load/unload a new texture
	virtual void Load(const char* pFilename);
	virtual void Unload();
	//==================================================

	// Function to get the Direct3D texture interface
	IDirect3DTexture9* GetTexture() const;

	// Functions for getting the texture dimensions
	int GetWidth() const;
	int GetHeight() const;

private:
	IDirect3DTexture9* mpTexture;  // Pointer to Direct3D texture interface

	int mWidth;
	int mHeight;
};

#endif // #ifndef INCLUDED_TEXTURE_H