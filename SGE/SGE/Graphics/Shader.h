#ifndef INCLUDED_SHADER_H
#define INCLUDED_SHADER_H

//====================================================================================================
// Filename:	Shader.h
// Description:	Class for a Direct3D effect.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Resource/Resource.h"

#include <d3dx9.h>

//====================================================================================================
// Class Declarations
//====================================================================================================

class Shader : public Resource
{
public:
	// Constructor
	Shader();

	// Destructor
	virtual ~Shader();

	//==================================================
	// Resource Implementation
	//==================================================
	// Function to load/unload a new effect
	virtual void Load(const char* pFilename);
	virtual void Unload();
	//==================================================

	// Function to get the Direct3D effect interface
	ID3DXEffect* GetEffect() const;

private:
	ID3DXEffect* mpEffect;
};

#endif // #ifndef INCLUDED_TEXTURE_H