#ifndef INCLUDED_POSTPROCESS_H
#define INCLUDED_POSTPROCESS_H

//====================================================================================================
// Filename:	PostProcess.h
// Description:	Class for post process render pass.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Shader.h"
#include "RenderSurface.h"

#include <d3dx9.h>

//====================================================================================================
// Class Declarations
//====================================================================================================

class PostProcess
{
public:
	// Functions to create/destroy the singleton
	static void Create();
	static void Destroy();

	// Accessor function for singleton instance
	static PostProcess* Get();

	// Destructor
	~PostProcess();

	// Functions to startup and shutdown the Post Process
	void Initialize(int winWidth, int winHeight, const char* filename);
	void Terminate();

	// Functions for rendering
	void BeginRender();
	void EndRender();

	// Accessor
	const Shader& GetShader() const	{ return mShader; }

protected:
	// Protected constructor for singleton
	PostProcess();

private:
	static PostProcess* s_pInstance;		// Static instance for singleton

	RenderSurface mRenderSurface;
	Shader mShader;

	int mWinWidth;
	int mWinHeight;
	bool mInitialized;
};

#endif // #ifndef INCLUDED_POSTPROCESS_H