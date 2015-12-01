#ifndef INCLUDED_SPRITE_H
#define INCLUDED_SPRITE_H

//====================================================================================================
// Filename:	Sprite.h
// Description:	Class for a 2D sprite. It contains an array of textures and provides functionalities
//				for transformations and animations.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <d3dx9.h>
#include <vector>

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Texture;

//====================================================================================================
// Class Declarations
//====================================================================================================

enum class BlendState
{
	Normal,
	Additive,
	Subtractive
};

class Sprite
{
public:
	// Constructor
	Sprite();

	// Destructor
	virtual ~Sprite();

	// Function to update the sprite
	virtual void Update(float fSeconds);

	// Function to render the sprite
	virtual void Render();

	// Functions for managing textures used by the sprite
	void AddTexture(Texture* pTexture);
	const Texture* GetTexture() const;
	void ClearTextures();

	// Function to get sprite dimension
	void GetDimension(int& iWidth, int& iHeight) const;

	// Function to set blend state
	void SetBlendState(BlendState state);

	// Functions to set color
	void SetColor(int iRed, int iGreen, int iBlue, int iAlpha);
	void SetColor(int argb);

	// Functions to set alpha
	void SetAlpha(int iAlpha);
	void SetAlpha(float fAlpha);

	// Functions to get sprite attributes
	void GetPosition(float& fPosX, float& fPosY) const;
	void GetScale(float& fScaleX, float& fScaleY) const;
	float GetRotation() const;
	float GetDepth() const;
	bool IsScaleAboutCenter() const;
	bool GetFlipH() const;
	bool GetFlipV() const;

	// Functions to set sprite attributes
	void SetPosition(float fPosX, float fPosY);
	void SetScale(float fScaleX, float fScaleY);
	void SetRotation(float fRotation);
	void SetDepth(float fDepth);
	void SetScaleAboutCenter(bool bScaleAboutCenter);
	void SetFlipH(bool bFlip);
	void SetFlipV(bool bFlip);

	// Functions for controlling animation playback
	void Play(bool bLoopOnce = false);
	void Stop();
	void Step();
	void Reset();

	// Function that returns true if the animation is done
	bool IsFinished() const;

	// Functions to get multi sprite attributes
	float GetAnimationSpeed() const;
	int GetCurrentFrame() const;

	// Functions to set multi sprite attributes
	void SetAnimationSpeed(float fSpeed);
	void SetCurrentFrame(int iFrame);

protected:
	// Members
	std::vector<Texture*> mpTextures;

	D3DXCOLOR mColor;

	D3DXVECTOR2 mPosition;
	D3DXVECTOR2 mScale;

	BlendState mBlendState;

	float mRotation;
	float mDepth;

	bool mScaleAboutCenter;
	bool mFlipH;
	bool mFlipV;

	float mAnimationSpeed;		// Frames per second
	float mAnimationTime;		// Time since start of animation
	int mCurrentFrame;			// Current animation frame

	bool mPlay;					// True when playing
	bool mLoopOnce;				// True if we only want to loop once
};

#endif // #ifndef INCLUDED_SPRITE_H