#ifndef INCLUDED_SGE_UI_H
#define INCLUDED_SGE_UI_H

//====================================================================================================
// Filename:	SGE_UI.h
// Description:	Header containing some useful classes for user interface.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <string>

#include "SGE_Math.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Font;
class Sprite;
class Texture;

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// Class Declarations
//====================================================================================================

class SGE_Font
{
public:
	SGE_Font();
	~SGE_Font();

	void Load(int iSize, bool bBold = false, bool bItalic = false);
	void Unload();

	void SetColor(int r, int g, int b);
	void Print(const char* pText, int iPositionX, int iPositionY);

private:
	Font* mpFont;
};

//----------------------------------------------------------------------------------------------------

class SGE_Cursor
{
public:
	SGE_Cursor();
	~SGE_Cursor();

	void Load(const char* pTextureName);
	void Unload();

	void Update(float dt);
	void Render();

	void Show();
	void Hide();

private:
	Sprite*	mpSprite;
	Texture*	mpTexture;
	bool		mShow;
};

//----------------------------------------------------------------------------------------------------

class SGE_Button
{
public:
	SGE_Button();
	~SGE_Button();

	void Load(const char* pTextureOn, const char* pTextureOff, int iTextSize = 24);
	void Unload();

	void Update(float fSeconds);
	void Render();

	void SetPosition(float x, float y);
	void SetPosition(const Vector2& pos);
	void SetText(const char* pText, int r, int g, int b);

	bool IsPressed() const;

private:
	enum{ ON = 0, OFF = 1 };

	Font*		mpFont;
	Sprite*		mpSprite;
	Texture*	mpTextureOn;
	Texture*	mpTextureOff;
	std::string	mText;
};

} // namespace SGE

#endif // #ifndef INCLUDED_SGE_UI_H