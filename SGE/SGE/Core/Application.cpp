//====================================================================================================
// Filename:	Application.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Core/Application.h"
#include "Core/Log.h"

#include <stdio.h>
#include <stdlib.h>

//====================================================================================================
// Class Definitions
//====================================================================================================

Application::Application()
	: mWindow(0)
	, mWinWidth(0)
	, mWinHeight(0)
	, mRequestQuit(false)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

Application::~Application()
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

void Application::Initialize(const char* pAppName, HINSTANCE hInstance, int iWinWidth, int iWinHeight)
{
	// Initialize error log
	Log::Create();
	Log::Get()->Initialize();

	// Write to log
	Log::Get()->Write(LogType::Message, "[Application] Initializing: %s...", pAppName);

	// Convert to wide-character app name
	size_t charsConverted = 0;
	wchar_t wideAppName[256];
	mbstowcs_s(&charsConverted, wideAppName, pAppName, 255);

	// Create a new window class
	WNDCLASSEX wc;

	// Fill the window class structure
	wc.cbSize			= sizeof(WNDCLASSEXA);
	wc.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= DefWindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= nullptr;
	wc.lpszClassName	= wideAppName;
	wc.hIconSm			= LoadIcon(nullptr, IDI_APPLICATION);

	// Register the window class
	RegisterClassEx(&wc);

	// Cache the windows dimension
	mWinWidth = iWinWidth;
	mWinHeight = iWinHeight;

	// Window style
	DWORD windowStyle = WS_EX_TOPMOST | WS_CAPTION | WS_SYSMENU;

	// Adjust the windows dimensions to account for the window frame
	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = iWinWidth;
	windowRect.bottom = iWinHeight;
	AdjustWindowRect(&windowRect, windowStyle, false);

	// Create a new window
	mWindow = CreateWindowEx(
		NULL,
		wideAppName,
		wideAppName,
		windowStyle,
		0, 0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// Hide the cursor
	//SetCursor(nullptr);

	// Show the window
	UpdateWindow(mWindow);
	ShowWindow(mWindow, SW_SHOWNORMAL);

	// Do any additional initialization here
	OnInitialize();

	// Write to log
	Log::Get()->Write(LogType::Message, "[Application] %s initialized", pAppName);
}

//----------------------------------------------------------------------------------------------------

void Application::Terminate()
{
	// Write to log
	Log::Get()->Write(LogType::Message, "[Application] Terminating...");

	// Do any additional termination tasks here
	OnTerminate();

	// Destroy the window
	DestroyWindow(mWindow);

	// Write to log
	Log::Get()->Write(LogType::Message, "[Application] Application terminated.");

	// Shutdown error log
	Log::Get()->Terminate();
	Log::Destroy();
}

//----------------------------------------------------------------------------------------------------

void Application::Run()
{
	MSG msg;
	memset(&msg, 0, sizeof(MSG));

	// Start the message loop
	while (	msg.message != WM_QUIT &&
			msg.message != WM_CLOSE)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// Detect Alt-F4 or other means of closing the window.
			if (msg.message == WM_SYSCOMMAND)
			{
				if (msg.wParam == SC_CLOSE)
				{
					break;
				}
			}
		}
		else
		{
			// Run main loop
			OnMainLoop();
		}

		// Handle request to quit the application.
		if (mRequestQuit == true)
		{
			Quit();
			return;
		}
	}
}

//----------------------------------------------------------------------------------------------------
void Application::RequestQuit()
{
	mRequestQuit = true;
}

//----------------------------------------------------------------------------------------------------

void Application::OnInitialize()
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

void Application::OnTerminate()
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

void Application::OnMainLoop()
{
	if (KEYDOWN(VK_ESCAPE))
	{
		RequestQuit();
	}
}

//----------------------------------------------------------------------------------------------------

void Application::Quit()
{
	PostQuitMessage(0);
}