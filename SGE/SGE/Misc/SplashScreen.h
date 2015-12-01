#ifndef INCLUDED_SPLASHSCREEN_H
#define INCLUDED_SPLASHSCREEN_H

//====================================================================================================
// Filename:	SplashScreen.h
// Description:	Class for creating a splash screen. Internally, it uses windows GDI for rendering and
//			  can display any BMP file passed into Initialize().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <windows.h>

//====================================================================================================
// Class Declarations
//====================================================================================================

class SplashScreen  
{
public:
	// Constructor
	SplashScreen();

	// Destructor
	virtual ~SplashScreen();

	// Functions to initialize/terminate the splash screen
	void Initialize(const char* pFilename);
	void Terminate();

	// Functions to show/hide the splash screen
	void Show(int iMilliSeconds = 0);
	void Hide();

	// Windows Message Handler
	LRESULT CALLBACK SplashWindowProc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);

private:
	// Function called when WM_PAINT message is received
	void OnPaint();

	// Members
	HWND	mSplashWindow;
	HBITMAP mBitmap;

	int mWinWidth;
	int mWinHeight;
};

#endif // #ifndef INCLUDED_SPLASHSCREEN_H
