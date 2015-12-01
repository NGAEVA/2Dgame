#ifndef INCLUDED_SPRITERENDERER_H
#define INCLUDED_SPRITERENDERER_H

//====================================================================================================
// Filename:	SpriteRenderer.h
// Description:	Singleton class for a sprite renderer. Internally it makes use of the ID3DXSprite
//			  interface for rendering. The singleton can be accessed calling SpriteRenderer::Get().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <d3dx9.h>

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Sprite;

//====================================================================================================
// Class Declarations
//====================================================================================================

class SpriteRenderer
{
public:
	// Functions to create/destroy the singleton
	static void Create();
	static void Destroy();

	// Accessor function for singleton instance
	static SpriteRenderer* Get();

	// Destructor
	virtual ~SpriteRenderer();

	// Functions to startup and shutdown the sprite renderer
	void Initialize();
	void Terminate();

	// Functions for rendering
	void BeginRender();
	void EndRender();

	// Function to render the ID3DXSprite interface
	ID3DXSprite* D3DXSprite() const;

protected:
	// Protected constructor for singleton
	SpriteRenderer();

private:
	// Members
	static SpriteRenderer* s_pInstance;		// Static instance for singleton

	ID3DXSprite* mpD3DXSprite;				 // Pointer to D3DX sprite interface

	bool mInitialized;						 // Init flag
};

#endif // #ifndef INCLUDED_SPRITERENDERER_H