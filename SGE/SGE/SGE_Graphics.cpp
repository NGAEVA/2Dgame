//====================================================================================================
// Filename:	SGE_Graphics.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "SGE_Graphics.h"

#include "Core/CoreSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Misc/Misc.h"
#include "Resource/ResourceSystem.h"
#include "Utility/UtilitySystem.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

class Particle
{
public:
	float px, py;
	float vx, vy;
	float lifetime;
	float size;
	int color;

	Particle()
		: px(0.0f), py(0.0f)
		, vx(0.0f), vy(0.0f)
		, lifetime(0.0f)
		, size(1.0f)
		, color(0xFFFFFFFF)
	{}
};

//====================================================================================================
// Helper Functions
//====================================================================================================

int BlendColor(float s, int c0, int c1)
{
	int r0, g0, b0;
	int r1, g1, b1;
	int rf, gf, bf;

	r0 = (c0 & 0x00FF0000) >> 16;
	g0 = (c0 & 0x0000FF00) >> 8;
	b0 = (c0 & 0x000000FF);

	r1 = (c1 & 0x00FF0000) >> 16;
	g1 = (c1 & 0x0000FF00) >> 8;
	b1 = (c1 & 0x000000FF);

	rf = r0 + static_cast<int>(s * (r1 - r0));
	gf = g0 + static_cast<int>(s * (g1 - g0));
	bf = b0 + static_cast<int>(s * (b1 - b0));

	return (0xFF << 24) | (rf << 16) | (gf << 8) | (bf);
}

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// Class Definitions
//====================================================================================================

SGE_Sprite::SGE_Sprite()
	: mpSprite(nullptr)
{
	mpSprite = new Sprite();
}

//----------------------------------------------------------------------------------------------------

SGE_Sprite::~SGE_Sprite()
{
	delete mpSprite;
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Load(const char* pTextureName)
{
	// Unload everything first
	Unload();

	// Construct the full filename
	std::string fullname;
	fullname += IniFile::Get()->GetString("TexturePath", "../Data/Textures/");
	fullname += pTextureName;

	// Load the texture
	Texture* pTexture = ResourceManager::Get()->Create<Texture>(fullname.c_str());
	mpTextures.push_back(pTexture);
	mpSprite->AddTexture(pTexture);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Add(const char* pTextureName)
{
	// Construct the full filename
	std::string fullname;
	fullname += IniFile::Get()->GetString("TexturePath", "../Data/Textures/");
	fullname += pTextureName;

	// Load the texture
	Texture* pTexture = ResourceManager::Get()->Create<Texture>(fullname.c_str());
	mpTextures.push_back(pTexture);
	mpSprite->AddTexture(pTexture);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Unload()
{
	// Clear texture reference
	mpSprite->ClearTextures();

	// Unload texture
	const int kNumTextures = mpTextures.size();
	for (int i = 0; i < kNumTextures; ++i)
	{
		if (nullptr != mpTextures[i])
		{
			ResourceManager::Get()->Destroy(mpTextures[i]);
			mpTextures[i] = nullptr;
		}
	}
	mpTextures.clear();
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Update(float fSeconds)
{
	mpSprite->Update(fSeconds);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Render()
{
	mpSprite->Render();
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetPosition(float x, float y)
{
	mpSprite->SetPosition(x, y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetPosition(const Vector2& pos)
{
	mpSprite->SetPosition(pos.x, pos.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetScale(float x, float y, bool scaleAboutCenter)
{
	mpSprite->SetScaleAboutCenter(scaleAboutCenter);
	mpSprite->SetScale(x, y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetScale(const Vector2& scale, bool scaleAboutCenter)
{
	mpSprite->SetScaleAboutCenter(scaleAboutCenter);
	mpSprite->SetScale(scale.x, scale.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetRotation(float rotation)
{
	mpSprite->SetRotation(rotation);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetFlipH(bool bFlip)
{
	mpSprite->SetFlipH(bFlip);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetFlipV(bool bFlip)
{
	mpSprite->SetFlipV(bFlip);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetAlpha(float alpha)
{
	mpSprite->SetAlpha(alpha);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetBlendState(BlendState state)
{
	mpSprite->SetBlendState(state);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Play(float speed, bool loop)
{
	mpSprite->SetAnimationSpeed(speed);
	mpSprite->Play(!loop);
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Stop()
{
	mpSprite->Stop();
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::Reset()
{
	mpSprite->Reset();
}

//----------------------------------------------------------------------------------------------------

int SGE_Sprite::GetWidth() const
{
	return mpSprite->GetTexture()->GetWidth();
}

//----------------------------------------------------------------------------------------------------

int SGE_Sprite::GetHeight() const
{
	return mpSprite->GetTexture()->GetHeight();
}

//----------------------------------------------------------------------------------------------------

bool SGE_Sprite::IsFinished() const
{
	return mpSprite->IsFinished();
}

//----------------------------------------------------------------------------------------------------

int SGE_Sprite::GetCurrentFrame() const
{
	return mpSprite->GetCurrentFrame();
}

//----------------------------------------------------------------------------------------------------

void SGE_Sprite::SetCurrentFrame(int iFrame)
{
	mpSprite->SetCurrentFrame(iFrame);
}

//----------------------------------------------------------------------------------------------------

SGE_Emitter::SGE_Emitter()
	: mpSprite(nullptr)
	, mpTexture(nullptr)
	, mParticles(nullptr)
	, mTimer(0.0f)
	, mRate(0.0f)
	, mInterval(1.0f)
	, mEmitted(0.0f)
	, mIndex(0)
	, mPosition(0.0f, 0.0f)
	, mOffset(0.0f, 0.0f)
	, mFire(false)
	, mBurst(false)
	, mMaxBufferSize(0)
	, mRegion(0.0f, 0.0f, 0.0f, 0.0f)
	, mDirection(0.0f, -1.0f)
	, mSpan(180.0f)
	, mSpeedMin(0.0f), mSpeedMax(0.0f)
	, mLifetime(1.0f)
	, mScaleMin(1.0f), mScaleMax(1.0f)
	, mStartColor(0xFFFFFFFF), mEndColor(0xFFFFFFFF)
{
	mpSprite = new Sprite();
}

//----------------------------------------------------------------------------------------------------

SGE_Emitter::~SGE_Emitter()
{
	delete mpSprite;
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::Load(const char* pEmitterName)
{
	// Unload everything first
	Unload();

	// Full name
	std::string fullname;
	fullname += IniFile::Get()->GetString("EmitterPath", "../Data/Emitters/");
	fullname += pEmitterName;

	// Open the emitter file
	FILE* pFile = nullptr;
	fopen_s(&pFile, fullname.c_str(), "r");
	if (nullptr == pFile)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Emitter] Failed to load emitter file %s.", pEmitterName);
		return;
	}

	// Validate the file
	const int kBufferSize = 256;
	char data[kBufferSize];
	fscanf_s(pFile, "%s", data, kBufferSize);
	if (0 != strcmp(data, "[EMITTER_DATA]"))
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[Emitter] Invalid emitter file %s.", fullname.c_str());
		fclose(pFile);
		return;
	}

	// Now read the data
	char texture[kBufferSize];
	int r0, g0, b0;
	int r1, g1, b1;

	// Read the emitter data
	fscanf_s(pFile, "%*s %s", texture, kBufferSize);
	fscanf_s(pFile, "%*s %i", &mMaxBufferSize);
	fscanf_s(pFile, "%*s %f, %f, %f, %f", &mRegion.min.x, &mRegion.min.y, &mRegion.max.x, &mRegion.max.y);
	fscanf_s(pFile, "%*s %f, %f", &mDirection.x, &mDirection.y);
	fscanf_s(pFile, "%*s %f", &mSpan);
	fscanf_s(pFile, "%*s %f, %f", &mSpeedMin, &mSpeedMax);
	fscanf_s(pFile, "%*s %f", &mLifetime);
	fscanf_s(pFile, "%*s %f, %f", &mScaleMin, &mScaleMax);
	fscanf_s(pFile, "%*s %d, %d, %d", &r0, &g0, &b0);
	fscanf_s(pFile, "%*s %d, %d, %d", &r1, &g1, &b1);

	// Close the data file
	fclose(pFile);

	// Pack color
	mStartColor = (0xFF << 24) | (r0 << 16) | (g0 << 8) | (b0);
	mEndColor = (0xFF << 24) | (r1 << 16) | (g1 << 8) | (b1);
	
	// Allocate memory for particles
	mParticles = new Particle[mMaxBufferSize];

	// Load and set texture
	fullname = IniFile::Get()->GetString("TexturePath", "../Data/Textures/");
	fullname += texture;
	mpTexture = ResourceManager::Get()->Create<Texture>(fullname.c_str());
	mpSprite->AddTexture(mpTexture);
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::Unload()
{
	// Clear texture reference
	mpSprite->ClearTextures();

	// Unload texture
	if (nullptr != mpTexture)
	{
		ResourceManager::Get()->Destroy(mpTexture);
		mpTexture = nullptr;
	}

	// Delete particles
	if (nullptr != mParticles)
	{
		delete[] mParticles;
		mParticles = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::Update(float fSeconds)
{
	if (mFire)
	{
		int iSlotSearched = 0;
		while (mEmitted <mRate && iSlotSearched <mMaxBufferSize)
		{
			Particle& p = mParticles[mIndex];

			// Check if slot is available
			if (p.lifetime <= 0.0f)
			{
				// Calculate parameters
				Vector2 vecPositionMin = mPosition + mRegion.min;
				Vector2 vecPositionMax = mPosition + mRegion.max;

				float fAngle = Random::GetFloat(-mSpan, mSpan);
				float fSpeed = Random::GetFloat(mSpeedMin, mSpeedMax);

				Vector2 vecDirection(mDirection);
				vecDirection.Rotate(Math_DegToRad(fAngle));
				vecDirection.Normalize();

				// Set particle attributes
				p.px = Random::GetFloat(vecPositionMin.x, vecPositionMax.x);
				p.py = Random::GetFloat(vecPositionMin.y, vecPositionMax.y);
				p.vx = vecDirection.x * fSpeed;
				p.vy = vecDirection.y * fSpeed;
				p.size = Random::GetFloat(mScaleMin, mScaleMax);
				p.color = mStartColor;
				p.lifetime = mLifetime;

				// Increment counter
				mEmitted += 1.0f;
			}

			// Advance index
			mIndex = (mIndex + 1) % mMaxBufferSize;
			++iSlotSearched;
		}

		// Check if this is burst mode
		if (mBurst)
		{
			// Reset
			mFire = false;
			mBurst = false;
			mTimer = 0.0f;
			mEmitted = 0.0f;
		}
		else
		{
			mTimer += fSeconds;

			if (mTimer> mInterval)
			{
				mTimer -= mInterval;
				mEmitted = 0.0f;
			}
		}
	}

	for (int i = 0; i < mMaxBufferSize; ++i)
	{
		Particle& p = mParticles[i];
		if (p.lifetime> 0.0f)
		{
			// Update lifetime
			p.lifetime -= fSeconds;

			float s = 1.0f - (p.lifetime / mLifetime);

			// Update attributes
			p.px += p.vx * fSeconds;
			p.py += p.vy * fSeconds;
			p.color = BlendColor(s, mStartColor, mEndColor);
		}
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::Render(bool bAdditiveBlend)
{
	if (bAdditiveBlend)
	{
		// Switch to additive blending
		SpriteRenderer::Get()->D3DXSprite()->Flush();
		DXGraphics::Get()->D3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		DXGraphics::Get()->D3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	for (int i = 0; i < mMaxBufferSize; ++i)
	{
		Particle& p = mParticles[i];
		if (p.lifetime > 0.0f)
		{
			mpSprite->SetPosition(p.px + mOffset.x, p.py + mOffset.y);
			mpSprite->SetScale(p.size, p.size);
			mpSprite->SetColor(p.color);
			mpSprite->Render();
		}
	}

	if (bAdditiveBlend)
	{
		// Revert to default blending
		SpriteRenderer::Get()->D3DXSprite()->Flush();
		DXGraphics::Get()->D3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		DXGraphics::Get()->D3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::Start(float rate, bool burst)
{
	if (burst)
	{
		mRate = rate;
		mInterval = 1.0f;
	}
	else
	{
		mRate = 1.0f;
		mInterval = 1.0f / rate;
	}

	mFire = true;
	mBurst = burst;
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::Stop()
{
	mTimer = 0.0f;
	mEmitted = 0.0f;

	mFire = false;
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::SetPosition(float x, float y)
{
	mPosition.x = x;
	mPosition.y = y;
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::SetPosition(const Vector2& pos)
{
	mPosition = pos;
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::SetOffset(float x, float y)
{
	mOffset.x = x;
	mOffset.y = y;
}

//----------------------------------------------------------------------------------------------------

void SGE_Emitter::SetOffset(Vector2 pos)
{
	mOffset = pos;
}

//----------------------------------------------------------------------------------------------------

SGE_WireFrame::SGE_WireFrame()
	: mWireFrame(nullptr)
{
	mWireFrame = new WireFrame();
	mWireFrame->Create();
}

//----------------------------------------------------------------------------------------------------

SGE_WireFrame::SGE_WireFrame(const SGE_WireFrame& rhs)
	: mWireFrame(nullptr)
{
	mWireFrame = new WireFrame(*rhs.mWireFrame);
	mWireFrame->Create();
}

//----------------------------------------------------------------------------------------------------

SGE_WireFrame::~SGE_WireFrame()
{
	mWireFrame->Destroy();
	delete mWireFrame;
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::operator=(const SGE_WireFrame& rhs)
{
	memcpy(&mWireFrame, &rhs.mWireFrame, sizeof(WireFrame));
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::Render()
{
	mWireFrame->Render();
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddLine(float x0, float y0, float x1, float y1)
{
	mWireFrame->AddLine(x0, y0, x1, y1);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddLine(const Vector2& start, const Vector2& end)
{
	mWireFrame->AddLine(start.x, start.y, end.x, end.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddRect(float x, float y, float w, float h)
{
	mWireFrame->AddRect(x, y, w, h);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddRect(const Vector2& pos, const Vector2& dim)
{
	mWireFrame->AddRect(pos.x, pos.y, dim.x, dim.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddRect(const Rect& rRect)
{
	mWireFrame->AddRect(rRect.min.x, rRect.min.y, rRect.max.x - rRect.min.x, rRect.max.y - rRect.min.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddCircle(float x, float y, float r)
{
	mWireFrame->AddCircle(x, y, r);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddCircle(const Vector2& c, float r)
{
	mWireFrame->AddCircle(c.x, c.y, r);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::AddCircle(const Circle& rCircle)
{
	mWireFrame->AddCircle(rCircle.center.x, rCircle.center.y, rCircle.radius);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::Purge()
{
	mWireFrame->Purge();
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::SetLineWidth(float fLineWidth)
{
	mWireFrame->SetLineWidth(fLineWidth);
}

//----------------------------------------------------------------------------------------------------

void SGE_WireFrame::SetColor(unsigned int rgb)
{
	mWireFrame->SetColor(rgb);
}


//====================================================================================================
// SGE Functions
//====================================================================================================

void Graphics_DebugPoint(const Vector2& vPosition, unsigned int rgb)
{
	Debug::AddScreenPoint(vPosition.x, vPosition.y, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugLine(const Vector2& vStart, const Vector2& vEnd, unsigned int rgb)
{
	Debug::AddScreenLine(vStart.x, vStart.y, vEnd.x, vEnd.y, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugLine(const LineSegment& rLineSegment, unsigned int rgb)
{
	Debug::AddScreenLine(rLineSegment.from.x, rLineSegment.from.y, rLineSegment.to.x, rLineSegment.to.y, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugRect(const Vector2& vPosition, const Vector2& vDimension, unsigned int rgb)
{
	float x = vPosition.x;
	float y = vPosition.y;
	float w = vDimension.x;
	float h = vDimension.y;
	Debug::AddScreenRect(x, y, w, h, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugRect(const Rect& rRect, unsigned int rgb)
{
	float x = rRect.min.x;
	float y = rRect.min.y;
	float w = rRect.max.x - rRect.min.x;
	float h = rRect.max.y - rRect.min.y;
	Debug::AddScreenRect(x, y, w, h, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugCircle(const Vector2& vCenter, float fRadius, unsigned int rgb)
{
	Debug::AddScreenCircle(vCenter.x, vCenter.y, fRadius, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugCircle(const Circle& rCircle, unsigned int rgb)
{
	Debug::AddScreenCircle(rCircle.center.x, rCircle.center.y, rCircle.radius, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugText(const char* pText, const Vector2& vPosition, unsigned int rgb)
{
	Debug::AddScreenText(pText, vPosition.x, vPosition.y, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugText(const char* pText, float x, float y, unsigned int rgb)
{
	Debug::AddScreenText(pText, x, y, rgb);
}

//----------------------------------------------------------------------------------------------------

void PostProcess_SetInt(const char* pName, int iValue)
{
	PostProcess::Get()->GetShader().GetEffect()->SetInt(pName, iValue);
}

//----------------------------------------------------------------------------------------------------

void PostProcess_SetIntArray(const char* pName, const int* iValues, int iCount)
{
	PostProcess::Get()->GetShader().GetEffect()->SetIntArray(pName, iValues, iCount);
}

//----------------------------------------------------------------------------------------------------

void PostProcess_SetFloat(const char* pName, float fValue)
{
	PostProcess::Get()->GetShader().GetEffect()->SetFloat(pName, fValue);
}

//----------------------------------------------------------------------------------------------------

void PostProcess_SetFloatArray(const char* pName, const float* fValues, int iCount)
{
	PostProcess::Get()->GetShader().GetEffect()->SetFloatArray(pName, fValues, iCount);
}

//----------------------------------------------------------------------------------------------------

void PostProcess_SetBool(const char* pName, BOOL bValue)
{
	PostProcess::Get()->GetShader().GetEffect()->SetBool(pName, bValue);
}

//----------------------------------------------------------------------------------------------------

void PostProcess_SetBoolArray(const char* pName, const BOOL* bValues, int iCount)
{
	PostProcess::Get()->GetShader().GetEffect()->SetBoolArray(pName, bValues, iCount);
}

//----------------------------------------------------------------------------------------------------

void PostProcess_SetVector(const char* pName, const D3DXVECTOR4& vector)
{
	PostProcess::Get()->GetShader().GetEffect()->SetVector(pName, &vector);
}

//----------------------------------------------------------------------------------------------------

void PostProcess_SetVectorArray(const char* pName, const D3DXVECTOR4* vectors, int iCount)
{
	PostProcess::Get()->GetShader().GetEffect()->SetVectorArray(pName, vectors, iCount);
}

} // namespace SGE