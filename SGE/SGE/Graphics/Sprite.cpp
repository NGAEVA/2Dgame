//====================================================================================================
// Filename:	Sprite.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics/Sprite.h"

#include "Core/Log.h"
#include "Graphics/SpriteRenderer.h"
#include "Graphics/Texture.h"
#include "Graphics/DXGraphics.h"
#include "Utility/Debug.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

Sprite::Sprite()
	: mColor(0xFFFFFFFF)
	, mPosition(0.0f, 0.0f)
	, mScale(1.0f, 1.0f)
	, mRotation(0.0f)
	, mDepth(0.0f)
	, mScaleAboutCenter(true)
	, mFlipH(false)
	, mFlipV(false)
	, mAnimationSpeed(0.0f)
	, mAnimationTime(0.0f)
	, mCurrentFrame(0)
	, mPlay(false)
	, mLoopOnce(false)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

Sprite::~Sprite()
{
	// Release everything
	ClearTextures();
}

//----------------------------------------------------------------------------------------------------

void Sprite::Update(float fSeconds)
{
	// Update the animation time
	if (mPlay)
	{
		mAnimationTime += mAnimationSpeed * fSeconds;
		
		// Calculate the current frame to use
		const int kNumFrames = static_cast<int>(mpTextures.size());
		if (kNumFrames > 0)
		{
			const int kFrameIndex = static_cast<int>(mAnimationTime * mAnimationSpeed);
			if (mLoopOnce)
			{
				if (kFrameIndex >= kNumFrames - 1)
				{
					mCurrentFrame = kNumFrames - 1;
				}
				else
				{
					mCurrentFrame = kFrameIndex;
				}
			}
			else
			{
				mCurrentFrame = kFrameIndex % kNumFrames;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------

void Sprite::Render()
{
	// Get the current texture to use
	const Texture* pTexture = GetTexture();

	// Check if we have a valid texture
	if (nullptr == pTexture)
	{
		return;
	}

	// Calculate texture center
	D3DXVECTOR2 vecTextureCenter(pTexture->GetWidth() * 0.5f, pTexture->GetHeight() * 0.5f);

	// Calculate sprite scale center
	D3DXVECTOR2 vecScaleCenter(0.0f, 0.0f);
	if (mScaleAboutCenter)
	{
		vecScaleCenter.x = vecTextureCenter.x;
		vecScaleCenter.y = vecTextureCenter.y;
	}

	// Calculate sprite rotation center
	D3DXVECTOR2 vecRotationCenter;
	vecRotationCenter.x = vecTextureCenter.x;
	vecRotationCenter.y = vecTextureCenter.y;
	if (!mScaleAboutCenter)
	{
		vecRotationCenter.x *= mScale.x;
		vecRotationCenter.y *= mScale.y;
	}

	// Create combined transform matrix
	D3DXMATRIX matCombined;
	D3DXMatrixTransformation2D(&matCombined,	   // Transform matrix
								&vecScaleCenter,	// Scaling center
								0.0f,			   // Scaling rotation
								&mScale,		   // Scaling
								&vecRotationCenter, // Rotation center
								mRotation,		 // Rotation
								&mPosition);	  // Translation

	// Apply flip
	D3DXMATRIX matPreFlip, matFlip, matPostFlip;
	D3DXMatrixTranslation(&matPreFlip, -vecTextureCenter.x, -vecTextureCenter.y, 0.0f);
	D3DXMatrixScaling(&matFlip, mFlipH ? -1.0f : 1.0f, mFlipV ? -1.0f : 1.0f, 1.0f);
	D3DXMatrixTranslation(&matPostFlip, vecTextureCenter.x, vecTextureCenter.y, 0.0f);
	
	// Add depth
	D3DXMATRIX matDepth;
	D3DXMatrixTranslation(&matDepth, 0.0f, 0.0f, mDepth);

	// Get the world transform
	D3DXMATRIX matWorld;
	matWorld = matPreFlip * matFlip * matPostFlip * matCombined * matDepth;

	// Set transform and render the sprite
	SpriteRenderer::Get()->D3DXSprite()->SetTransform(&matWorld);

	RECT srcRect;
	srcRect.left = 0;
	srcRect.top = 0;
	srcRect.right = pTexture->GetWidth();
	srcRect.bottom = pTexture->GetHeight();

	// Draw the sprite texture.
	SpriteRenderer::Get()->D3DXSprite()->Draw(pTexture->GetTexture(), &srcRect, nullptr, nullptr, mColor);

#if 0
	// Draw original
	D3DXVECTOR2 pos(mPosition);
	D3DXVECTOR2 wh((float)pTexture->GetWidth(), (float)pTexture->GetHeight());
	Debug::AddScreenRect(pos.x, pos.y, wh.x, wh.y, 0xFFFFFF);

	// Draw center
	Debug::AddScreenLine(pos.x - 5.0f, pos.y - 5.0f, pos.x + 5.0f, pos.y + 5.0f, 0x8888FF);
	Debug::AddScreenLine(pos.x + 5.0f, pos.y - 5.0f, pos.x - 5.0f, pos.y + 5.0f, 0x8888FF);

	// Draw transformed
	D3DXVECTOR2 A(0.0f, 0.0f);
	D3DXVECTOR2 B(wh.x, 0.0f);
	D3DXVECTOR2 C(0.0f, wh.y);
	D3DXVECTOR2 D(wh.x, wh.y);
	D3DXVec2TransformCoord(&A, &A, &matCombined);
	D3DXVec2TransformCoord(&B, &B, &matCombined);
	D3DXVec2TransformCoord(&C, &C, &matCombined);
	D3DXVec2TransformCoord(&D, &D, &matCombined);
	Debug::AddScreenLine(A.x, A.y, B.x, B.y, 0xFF69B4);
	Debug::AddScreenLine(A.x, A.y, C.x, C.y, 0xFF69B4);
	Debug::AddScreenLine(D.x, D.y, B.x, B.y, 0xFF69B4);
	Debug::AddScreenLine(D.x, D.y, C.x, C.y, 0xFF69B4);

	// Draw transformed center
	D3DXVECTOR2 pos2(mPosition + vecRotationCenter);
	Debug::AddScreenLine(pos2.x - 5.0f, pos2.y - 5.0f, pos2.x + 5.0f, pos2.y + 5.0f, 0x88FFFF);
	Debug::AddScreenLine(pos2.x + 5.0f, pos2.y - 5.0f, pos2.x - 5.0f, pos2.y + 5.0f, 0x88FFFF);
#endif
}

//----------------------------------------------------------------------------------------------------

void Sprite::AddTexture(Texture* pTexture)
{
	mpTextures.push_back(pTexture);
}

//----------------------------------------------------------------------------------------------------

const Texture* Sprite::GetTexture() const
{
	Texture* pTexture = nullptr;
	if (mpTextures.size() > 0)
	{
		pTexture = mpTextures[mCurrentFrame];
	}
	return pTexture;
}

//----------------------------------------------------------------------------------------------------

void Sprite::ClearTextures()
{
	mpTextures.clear();
}

//----------------------------------------------------------------------------------------------------

void Sprite::GetDimension(int& iWidth, int& iHeight) const
{
	// Get the current frame to use
	const Texture* pTexture = GetTexture();
	if (nullptr != pTexture)
	{
		iWidth = static_cast<int>(pTexture->GetWidth() * mScale.x);
		iHeight = static_cast<int>(pTexture->GetHeight() * mScale.y);
	}
	else
	{
		iWidth = 0;
		iHeight = 0;
	}
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetBlendState(BlendState state)
{
	mBlendState = state;
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetColor(int iRed, int iGreen, int iBlue, int iAlpha)
{
	mColor = D3DCOLOR_RGBA(iRed, iGreen, iBlue, iAlpha);
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetColor(int argb)
{
	mColor = argb;
}
//----------------------------------------------------------------------------------------------------

void Sprite::SetAlpha(int iAlpha)
{
	mColor.a = (iAlpha & 0xff) / 255.0f;
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetAlpha(float fAlpha)
{
	mColor.a = fAlpha;
}

//----------------------------------------------------------------------------------------------------

void Sprite::GetPosition(float& fPosX, float& fPosY) const
{
	fPosX = mPosition.x;
	fPosY = mPosition.y;
}

//----------------------------------------------------------------------------------------------------

void Sprite::GetScale(float& fScaleX, float& fScaleY) const
{
	fScaleX = mScale.x;
	fScaleY = mScale.y;
}

//----------------------------------------------------------------------------------------------------

float Sprite::GetRotation() const
{
	return mRotation;
}

//----------------------------------------------------------------------------------------------------

float Sprite::GetDepth() const
{
	return mDepth;
}

//----------------------------------------------------------------------------------------------------

bool Sprite::IsScaleAboutCenter() const
{
	return mScaleAboutCenter;
}

//----------------------------------------------------------------------------------------------------

bool Sprite::GetFlipH() const
{
	return mFlipH;
}

//----------------------------------------------------------------------------------------------------

bool Sprite::GetFlipV() const
{
	return mFlipV;
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetPosition(float fPosX, float fPosY)
{
	mPosition.x = fPosX;
	mPosition.y = fPosY;
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetScale(float fScaleX, float fScaleY)
{
	mScale.x = fScaleX;
	mScale.y = fScaleY;
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetRotation(float fRotation)
{
	mRotation = fRotation;
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetDepth(float fDepth)
{
	if (fDepth > 1.0f)
	{
		mDepth = 1.0f;
	}
	else if (fDepth < 0.0f)
	{
		mDepth = 0.0f;
	}
	else
	{
		mDepth = fDepth;
	}
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetScaleAboutCenter(bool bScaleAboutCenter)
{
	mScaleAboutCenter = bScaleAboutCenter;
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetFlipH(bool bFlip)
{
	mFlipH = bFlip;
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetFlipV(bool bFlip)
{
	mFlipV = bFlip;
}

//----------------------------------------------------------------------------------------------------

void Sprite::Play(bool bLoopOnce)
{
	mPlay = true;
	mLoopOnce = bLoopOnce;
}

//----------------------------------------------------------------------------------------------------

void Sprite::Stop()
{
	mPlay = false;
}

//----------------------------------------------------------------------------------------------------

void Sprite::Step()
{
	const int kNumFrames = static_cast<int>(mpTextures.size());
	if (kNumFrames > 0)
	{
		mCurrentFrame = ++mCurrentFrame % kNumFrames;
	}
}

//----------------------------------------------------------------------------------------------------

void Sprite::Reset()
{
	mAnimationTime = 0.0f;
	mCurrentFrame = 0;
}

//----------------------------------------------------------------------------------------------------

bool Sprite::IsFinished() const
{
	if (!mLoopOnce)
	{
		return false;
	}

	const int kNumFrames = static_cast<int>(mpTextures.size());
	if (kNumFrames <= 0)
	{
		return false;
	}

	const float durationPerFrame = 1.0f / mAnimationSpeed;
	const float duration = durationPerFrame * kNumFrames;
	return mAnimationTime >= duration;
}

//----------------------------------------------------------------------------------------------------

float Sprite::GetAnimationSpeed() const
{
	return mAnimationSpeed;
}

//----------------------------------------------------------------------------------------------------

int Sprite::GetCurrentFrame() const
{
	return mCurrentFrame;
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetAnimationSpeed(float fSpeed)
{
	mAnimationSpeed = fSpeed;
}

//----------------------------------------------------------------------------------------------------

void Sprite::SetCurrentFrame(int iFrame)
{
	const int kNumFrames = static_cast<int>(mpTextures.size());
	if (iFrame < kNumFrames)
	{
		mCurrentFrame = iFrame;
	}
	else
	{
		mCurrentFrame = kNumFrames - 1;
	}
}