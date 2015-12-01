#ifndef INCLUDED_APPLICATION_H
#define INCLUDED_APPLICATION_H

//====================================================================================================
// Filename:	Application.h
// Description:	Class for creating a windows application. Any new project can inherit from
//			  Application and extend functionality by giving new implementations to OnInitialize(),
//			  OnTerminate(), OnMainLoop().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#define WIN32_LEAN_AND_MEAN	// Reduce windows include scope
#include <windows.h>

//====================================================================================================
// Defines
//====================================================================================================

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//====================================================================================================
// Class Declarations
//====================================================================================================

class Application
{
public:
	// Constructor
	Application();

	// Destructor
	virtual ~Application();

	// Functions to startup and shutdown the application
	void Initialize(const char* pAppName, HINSTANCE hInstance, int iWinWidth, int iWinHeight);
	void Terminate();

	// Function to enter main loop
	void Run();

	// Function to be called when we want to quit the application
	void RequestQuit();

protected:
	// Customizable functions for specific tasks
	virtual void OnInitialize();
	virtual void OnTerminate();
	virtual void OnMainLoop();

	HWND mWindow;
	int mWinWidth;
	int mWinHeight;

private:
	void Quit();
	
	bool mRequestQuit;
};

#endif // #ifndef INCLUDED_APPLICATION_H