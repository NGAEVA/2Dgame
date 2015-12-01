#ifndef INCLUDED_SGE_GRAPHICS_H
#define INCLUDED_SGE_GRAPHICS_H
//123
//====================================================================================================
// Filename:	SGE_Graphics.h
// Description:	Header containing some useful classes for graphics rendering.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <vector>
#include <d3dx9.h>

#include "SGE_Math.h"
#include "Graphics\Sprite.h"

//====================================================================================================
// Typedefs
//====================================================================================================

typedef unsigned int COLOR;

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Particle;
class Sprite;
class Texture;
class WireFrame;

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// Class Declarations
//====================================================================================================

class SGE_Sprite
{
public:
	SGE_Sprite();
	~SGE_Sprite();

	void Load(const char* pTextureName);
	void Add(const char* pTextureName);
	void Unload();

	void Update(float fSeconds);
	void Render();

	void SetPosition(float x, float y);
	void SetPosition(const Vector2& pos);
	void SetScale(float x, float y, bool scaleAboutCenter = true);
	void SetScale(const Vector2& scale, bool scaleAboutCenter = true);
	void SetRotation(float rotation);
	void SetFlipH(bool bFlip);
	void SetFlipV(bool bFlip);
	void SetAlpha(float alpha);
	void SetBlendState(BlendState state);

	void Play(float speed, bool loop);
	void Stop();
	void Reset();

	int GetWidth() const;
	int GetHeight() const;

	bool IsFinished() const;

	int GetCurrentFrame() const;
	void SetCurrentFrame(int iFrame);

private:
	std::vector<Texture*> mpTextures;
	Sprite*				 mpSprite;
};

//----------------------------------------------------------------------------------------------------

class SGE_Emitter
{
public:
	SGE_Emitter();
	~SGE_Emitter();

	void Load(const char* pEmitterName);
	void Unload();

	void Update(float fSeconds);
	void Render(bool bAdditiveBlend = false);

	void Start(float rate, bool burst);
	void Stop();

	void SetPosition(float x, float y);
	void SetPosition(const Vector2& pos);

	void SetOffset(float x, float y);
	void SetOffset(Vector2 pos);

private:
	Particle*	mParticles;
	Sprite*	mpSprite;
	Texture*	mpTexture;

	// Emission Attributes
	float	mTimer;
	float	mRate;
	float	mInterval;
	float	mEmitted;
	int		mIndex;

	// Attributes set in game
	Vector2	mPosition;
	Vector2	mOffset;
	bool		mFire;
	bool		mBurst;

	// Attributes set from script
	int			mMaxBufferSize;
	Rect		mRegion;
	Vector2	mDirection;
	float		mSpan;
	float		mSpeedMin, mSpeedMax;
	float		mLifetime;
	float		mScaleMin, mScaleMax;
	COLOR		mStartColor, mEndColor;
};

//----------------------------------------------------------------------------------------------------

class SGE_WireFrame
{
public:
	SGE_WireFrame();
	SGE_WireFrame(const SGE_WireFrame&);
	~SGE_WireFrame();
	void operator=(const SGE_WireFrame&);

	void Render();

	void AddLine(float x0, float y0, float x1, float y1);
	void AddLine(const Vector2& start, const Vector2& end);
	void AddRect(float x, float y, float w, float h);
	void AddRect(const Vector2& pos, const Vector2& dim);
	void AddRect(const Rect& rRect);
	void AddCircle(float x, float y, float r);
	void AddCircle(const Vector2& c, float r);
	void AddCircle(const Circle& rCircle);

	void Purge();

	void SetLineWidth(float fLineWidth);
	void SetColor(unsigned int rgb);

private:
	WireFrame* mWireFrame;
};


//====================================================================================================
// SGE Functions
//====================================================================================================

// Graphics System Functions
void Graphics_DebugPoint(const Vector2& vPosition, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugLine(const Vector2& vStart, const Vector2& vEnd, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugLine(const LineSegment& rLineSegment, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugRect(const Vector2& vPosition, const Vector2& vDimension, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugRect(const Rect& rRect, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugCircle(const Vector2& vCenter, float fRadius, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugCircle(const Circle& rCircle, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugText(const char* pText, const Vector2& vPosition, unsigned int rgb = 0xFFFFFF);
void Graphics_DebugText(const char* pText, float x, float y, unsigned int rgb = 0xFFFFFF);

// PostProcess Functions
void PostProcess_SetInt(const char* pName, int iValue);
void PostProcess_SetIntArray(const char* pName, const int* iValues, int iCount);
void PostProcess_SetFloat(const char* pName, float fValue);
void PostProcess_SetFloatArray(const char* pName, const float* fValues, int iCount);
void PostProcess_SetBool(const char* pName, BOOL bValue);
void PostProcess_SetBoolArray(const char* pName, const BOOL* bValues, int iCount);
void PostProcess_SetVector(const char* pName, const D3DXVECTOR4& vector);
void PostProcess_SetVectorArray(const char* pName, const D3DXVECTOR4* bValues, int iCount);

} // namespace SGE

#endif // #ifndef INCLUDED_SGE_GRAPHICS_H
