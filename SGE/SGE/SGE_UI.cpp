//====================================================================================================
// Filename:	SGE_Graphics.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "SGE_UI.h"

#include "Graphics/GraphicsSystem.h"
#include "Input/InputSystem.h"
#include "Misc/Misc.h"
#include "Resource/ResourceSystem.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// Class Definitions
//====================================================================================================

SGE_Font::SGE_Font()
	: mpFont(nullptr)
{
	mpFont = new Font();
}

//----------------------------------------------------------------------------------------------------

SGE_Font::~SGE_Font()
{
	delete mpFont;
}

//----------------------------------------------------------------------------------------------------

void SGE_Font::Load(int iSize, bool bBold, bool bItalic)
{
	mpFont->Create(FontType::TAHOMA, iSize, bBold, bItalic);
}

//----------------------------------------------------------------------------------------------------

void SGE_Font::Unload()
{
	mpFont->Destroy();
}

//----------------------------------------------------------------------------------------------------

void SGE_Font::SetColor(int r, int g, int b)
{
	mpFont->SetColor(r, g, b);
}

//----------------------------------------------------------------------------------------------------

void SGE_Font::Print(const char* pText, int iPositionX, int iPositionY)
{
	mpFont->PrintText(pText, iPositionX, iPositionY);
}

//----------------------------------------------------------------------------------------------------

SGE_Cursor::SGE_Cursor()
	: mpSprite(nullptr)
	, mpTexture(nullptr)
	, mShow(true)
{
	mpSprite = new Sprite();
}

//----------------------------------------------------------------------------------------------------

SGE_Cursor::~SGE_Cursor()
{
	delete mpSprite;
}

//----------------------------------------------------------------------------------------------------

void SGE_Cursor::Load(const char* pTextureName)
{
	// Unload everything first
	Unload();

	// Construct the full filename
	std::string fullname;
	fullname += IniFile::Get()->GetString("TexturePath", "../Data/Textures/");
	fullname += pTextureName;

	// Load the texture
	mpTexture = ResourceManager::Get()->Create<Texture>(fullname.c_str());
	mpSprite->AddTexture(mpTexture);
}

//----------------------------------------------------------------------------------------------------

void SGE_Cursor::Unload()
{
	// Clear texture reference
	mpSprite->ClearTextures();

	// Unload texture
	if (nullptr != mpTexture)
	{
		ResourceManager::Get()->Destroy(mpTexture);
		mpTexture = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Cursor::Update(float dt)
{
	float fMouseX = (float)DXInput::Get()->GetMouseScreenX();
	float fMouseY = (float)DXInput::Get()->GetMouseScreenY();
	mpSprite->SetPosition(fMouseX, fMouseY);
}

//----------------------------------------------------------------------------------------------------

void SGE_Cursor::Render()
{
	if (mShow)
	{
		mpSprite->Render();
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Cursor::Show()
{
	mShow = true;
}

//----------------------------------------------------------------------------------------------------

void SGE_Cursor::Hide()
{
	mShow = false;
}

//----------------------------------------------------------------------------------------------------

SGE_Button::SGE_Button()
	: mpFont(nullptr)
	, mpSprite(nullptr)
	, mpTextureOn(nullptr)
	, mpTextureOff(nullptr)
{
	mpFont = new Font();
	mpSprite = new Sprite();
}

//----------------------------------------------------------------------------------------------------

SGE_Button::~SGE_Button()
{
	delete mpSprite;
	delete mpFont;
}

//----------------------------------------------------------------------------------------------------


void SGE_Button::Load(const char* pTextureOn, const char* pTextureOff, int iTextSize)
{
	// Unload everything first
	Unload();

	// Construct the full filename
	std::string path;
	path = IniFile::Get()->GetString("TexturePath", "../Data/Textures/");

	// Load textures
	std::string onTextureName;
	onTextureName = path + pTextureOn;
	mpTextureOn = ResourceManager::Get()->Create<Texture>(onTextureName.c_str());

	std::string offTextureName;
	offTextureName = path + pTextureOff;
	mpTextureOff = ResourceManager::Get()->Create<Texture>(offTextureName.c_str());

	// Add textures to sprite
	mpSprite->AddTexture(mpTextureOn);
	mpSprite->AddTexture(mpTextureOff);

	// Create font
	mpFont->Create(FontType::TAHOMA, iTextSize);
	mpFont->SetFormat(FontFormat::CENTER | FontFormat::MIDDLE | FontFormat::SINGLE);
	mpFont->SetTextArea(0, 0, mpTextureOn->GetWidth(), mpTextureOn->GetHeight());
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::Unload()
{
	// Destroy font
	mpFont->Destroy();

	// Clear texture reference
	mpSprite->ClearTextures();

	// Unload textures
	if (nullptr != mpTextureOn)
	{
		ResourceManager::Get()->Destroy(mpTextureOn);
		mpTextureOn = nullptr;
	}
	if (nullptr != mpTextureOff)
	{
		ResourceManager::Get()->Destroy(mpTextureOff);
		mpTextureOff = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::Update(float fSeconds)
{
	const Texture* pTexture = mpSprite->GetTexture();

	Rect boundary;
	mpSprite->GetPosition(boundary.min.x, boundary.min.y);
	boundary.max.x = boundary.min.x + pTexture->GetWidth();
	boundary.max.y = boundary.min.y + pTexture->GetHeight();

	Vector2 mouse((float)DXInput::Get()->GetMouseScreenX(), (float)DXInput::Get()->GetMouseScreenY());
	if (Math_PointInRect(mouse, boundary))
	{
		mpSprite->SetCurrentFrame(ON);
	}
	else
	{
		mpSprite->SetCurrentFrame(OFF);
	}
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::Render()
{
	mpSprite->Render();

	Vector2 vPos;
	mpSprite->GetPosition(vPos.x, vPos.y);
	mpFont->PrintText(mText.c_str(), (int)vPos.x, (int)vPos.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::SetPosition(float x, float y)
{
	mpSprite->SetPosition(x, y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::SetPosition(const Vector2& pos)
{
	mpSprite->SetPosition(pos.x, pos.y);
}

//----------------------------------------------------------------------------------------------------

void SGE_Button::SetText(const char* pText, int r, int g, int b)
{
	mText = pText;
	mpFont->SetColor(r, g, b);
}

//----------------------------------------------------------------------------------------------------

bool SGE_Button::IsPressed() const
{
	const Texture* pTexture = mpSprite->GetTexture();

	Rect boundary;
	mpSprite->GetPosition(boundary.min.x, boundary.min.y);
	boundary.max.x = boundary.min.x + pTexture->GetWidth();
	boundary.max.y = boundary.min.y + pTexture->GetHeight();

	bool bPressed = false;
	Vector2 mouse((float)DXInput::Get()->GetMouseScreenX(), (float)DXInput::Get()->GetMouseScreenY());
	if (Math_PointInRect(mouse, boundary) && DXInput::Get()->IsMousePressed(Mouse::LBUTTON))
	{
		bPressed = true;
	}
	return bPressed;
}

} // namespace SGE