#ifndef INCLUDED_SGE_H
#define INCLUDED_SGE_H

//====================================================================================================
// Filename:	SGE.h
// Description:	Simple Game Engine.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <stdio.h>
#include <d3dx9.h>

#include "SGE_Audio.h"
#include "SGE_Graphics.h"
#include "SGE_Input.h"
#include "SGE_Math.h"
#include "SGE_Physics.h"
#include "SGE_UI.h"

#include "Core/Application.h"
#include "Input/InputTypes.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class Font;

class Game : public Application
{
public:
	Game();
	virtual ~Game();

protected:
	// User override functions
	virtual void OnInitializeImpl() {}
	virtual void OnTerminateImpl() {}
	virtual bool OnUpdateImpl(float dt) { return true; }
	virtual void OnRenderImpl(float dt) {}

private:
	// Application interface
	virtual void OnInitialize();
	virtual void OnTerminate();
	virtual void OnMainLoop();

	void HandlePause();
	void UpdatePass(float dt);
	void RenderPass(float dt);

	Font* mFont;
	bool mDebug;
	bool mFPS;
	bool mPostProcessing;
	bool mDebugStepFrame;
	bool mPaused;
};

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// SGE Functions
//====================================================================================================

// Error Log Functions
void		LogMessage(const char* msg, ...);
void		LogWarning(const char* msg, ...);
void		LogError(const char* msg, ...);

// INI File Functions
int			IniFile_GetInt(const char* pTag, int iDefault);
bool		IniFile_GetBool(const char* pTag, bool bDefault);
float		IniFile_GetFloat(const char* pTag, float fDefault);
const char*	IniFile_GetString(const char* pTag, const char* pDefault);

// Random Functions
int			Random_GetInt(int iLower, int iUpper);
float		Random_GetFloat(float fLower, float fUpper);
Vector2		Random_GetVector2(const Vector2& vLower, const Vector2& vUpper);

} // namespace SGE

#endif // #ifndef INCLUDED_SGE_H