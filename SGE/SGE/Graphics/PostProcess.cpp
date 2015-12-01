//====================================================================================================
// Filename:	PostProcess.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics/PostProcess.h"

#include <assert.h>

#include "Core/Log.h"
#include "Graphics/DXGraphics.h"

//====================================================================================================
// Statics
//====================================================================================================

PostProcess* PostProcess::s_pInstance = nullptr;

//====================================================================================================
// Class Definitions
//====================================================================================================

void PostProcess::Create()
{
	assert(s_pInstance == nullptr);
	s_pInstance = new PostProcess();
}

//----------------------------------------------------------------------------------------------------

void PostProcess::Destroy()
{
	assert(s_pInstance != nullptr);
	delete s_pInstance;
	s_pInstance = nullptr;
}

//----------------------------------------------------------------------------------------------------

PostProcess* PostProcess::Get()
{
	assert(s_pInstance != nullptr);
	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

PostProcess::PostProcess() :
	mRenderSurface(),
	mShader(),
	mInitialized(false)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

PostProcess::~PostProcess()
{
	// Terminate the post process
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void PostProcess::Initialize(int winWidth, int winHeight, const char* filename)
{
	// Check if we have already initialized the post process
	if (mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[PostProcess] PostProcess already initialized.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[PostProcess] Initializing...");

	mWinWidth = winWidth;
	mWinHeight = winHeight;

	mRenderSurface.Create(winWidth, winHeight);
	mShader.Load(filename);

	// Write to log
	Log::Get()->Write(LogType::Message, "[PostProcess] PostProcess initialized.");

	// Set flag
	mInitialized = true;
}

//----------------------------------------------------------------------------------------------------

void PostProcess::Terminate()
{
	// Check if we have already terminated the renderer
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[PostProcess] PostProcess already terminated.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[PostProcess] Terminating...");

	mShader.Unload();
	mRenderSurface.Destroy();

	// Write to log
	Log::Get()->Write(LogType::Message, "[PostProcess] PostProcess terminated.");

	// Set flag
	mInitialized = false;
}

//----------------------------------------------------------------------------------------------------

void PostProcess::BeginRender()
{
	if (mInitialized)
	{
		mRenderSurface.Bind();
	}
}

//----------------------------------------------------------------------------------------------------

void PostProcess::EndRender()
{
	if (mInitialized)
	{
		ID3DXEffect* effect = mShader.GetEffect();

		// Unbind the current render surface first.
		mRenderSurface.UnBind();

		// Begin scene
		DXGraphics::Get()->BeginRender();

		// Set technique and texture
		effect->SetTechnique("PostProcess");
		effect->SetTexture("Texture", mRenderSurface.GetTexture());
			
		// Draw a screen sized quad
		struct Vertex
		{
			float x, y, z, w, u, v;
		};
		Vertex vertices[] =
		{
			{ -0.5f,             -0.5f,              1.0f, 1.0f, 0.0f, 0.0f },
			{ -0.5f + mWinWidth, -0.5f,              1.0f, 1.0f, 1.0f, 0.0f },
			{ -0.5f,             -0.5f + mWinHeight, 1.0f, 1.0f, 0.0f, 1.0f },
			{ -0.5f + mWinWidth, -0.5f + mWinHeight, 1.0f, 1.0f, 1.0f, 1.0f }
		};
		effect->Begin(0, 0);
		effect->BeginPass(0);
		DXGraphics::Get()->D3DDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
		DXGraphics::Get()->D3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (const void*)&vertices[0], sizeof(Vertex));
		effect->EndPass();
		effect->End();

		// Clear technique and texture
		effect->SetTexture("Texture", nullptr);
		effect->SetTechnique(nullptr);
	}
}
