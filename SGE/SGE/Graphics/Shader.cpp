//====================================================================================================
// Filename:	Shader.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Shader.h"

#include "Core/Log.h"
#include "Graphics/DXGraphics.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

Shader::Shader()
	: mpEffect(nullptr)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

Shader::~Shader()
{
	// Release everything
	Unload();
}

//----------------------------------------------------------------------------------------------------

void Shader::Load(const char* pFilename)
{
	// Clear everything before we create the effect
	Unload();

	// Load effect
	ID3DXBuffer* pErrorBuffer = nullptr;
	D3DXCreateEffectFromFileA
	(
		DXGraphics::Get()->D3DDevice(),	// Direct3D device interface
		pFilename,							// Filename
		nullptr,								// Macros
		nullptr,								// Includes
		D3DXSHADER_DEBUG,					// Compile flags
		nullptr,								// Effect pool
		&mpEffect,							// Pointer to effect interface
		&pErrorBuffer						// Error buffer
	);

	if (nullptr != pErrorBuffer)
	{
		// Show error dialog
		MessageBoxA
		(
			nullptr,
			(const char*)pErrorBuffer->GetBufferPointer(),
			"Shader Error",
			MB_OK | MB_ICONERROR
		);
		
		// Write to log
		Log::Get()->Write(LogType::Error, "[Shader] Failed to compile shader %s", pFilename);

		pErrorBuffer->Release();
		return;
	}

	// Check if we have any errors
	if (nullptr == mpEffect)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Shader] Failed to create shader from file %s", pFilename);
		return;
	}
}

//----------------------------------------------------------------------------------------------------

void Shader::Unload()
{
	// Release everything
	if (nullptr != mpEffect)
	{
		mpEffect->Release();
		mpEffect = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------

ID3DXEffect* Shader::GetEffect() const
{
	return mpEffect;
}