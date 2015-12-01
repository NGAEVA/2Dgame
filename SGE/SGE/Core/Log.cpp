//====================================================================================================
// Filename:	Log.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Log.h"

#include <cassert>
#include <cstdio>
#include <ctime>

//====================================================================================================
// Defines
//====================================================================================================

#if defined( _DEBUG )
#define ENABLE_LOG
#endif

//====================================================================================================
// Constants
//====================================================================================================

const char* c_LogMessageColour[LogType::MAX] =
{
	"#FFFFFF",  // Message,
	"#FFFF00",  // Warning,
	"#FF0000"   // Error
};

//====================================================================================================
// Statics
//====================================================================================================

Log* Log::s_pInstance = nullptr;

//====================================================================================================
// Class Definitions
//====================================================================================================

void Log::Create()
{
	assert(s_pInstance == nullptr);
	s_pInstance = new Log();
}

//----------------------------------------------------------------------------------------------------

void Log::Destroy()
{
	assert(s_pInstance != nullptr);
	delete s_pInstance;
	s_pInstance = nullptr;
}

//----------------------------------------------------------------------------------------------------

Log* Log::Get()
{
	assert(s_pInstance != nullptr);
	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

Log::Log()
	: mpFilename(nullptr)
	, mInitialized(false)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

void Log::Initialize(const char* pFilename)
{
#if defined(ENABLE_LOG)
	// Open a file for writing
	FILE* pFile = nullptr;
	fopen_s(&pFile, pFilename, "wt");

	// Check if we can open a file
	if (nullptr == pFile)
	{
		MessageBoxA(nullptr, "Error: Cannot open file for error log", "Log", MB_OK | MB_ICONERROR);
		return;
	}

	// Write the header for our log file
	fprintf_s(pFile, "<HTML>\n<HEAD><TITLE>Application Log</TITLE></HEAD>\n<BODY BGCOLOR = \"#000000\">\n");

	// Get time and date
	char time[32];
	char date[32];
	_strtime_s(time, 32);
	_strdate_s(date, 32);

	// Write the time and date into the file
	fprintf_s(pFile, "<FONT COLOR = \"#FFFFFF\">Log Started at %s on %s</FONT><BR><BR>\n", time, date);
	fprintf_s(pFile, "</BODY></HTML>");

	// Close the file
	fclose(pFile);

	// Remember the file name for logging later
	mpFilename = const_cast<char*>(pFilename);

	// Set flag
	mInitialized = true;
#endif
}

//----------------------------------------------------------------------------------------------------

void Log::Terminate()
{
#if defined(ENABLE_LOG)
	// Clear the file name
	mpFilename = nullptr;

	// Set flag
	mInitialized = false;
#endif
}

//----------------------------------------------------------------------------------------------------

void Log::Write(LogType logType, const char* pMessage, ...)
{
#if defined(ENABLE_LOG)
	va_list args;
	va_start(args, pMessage);
	Write(logType, pMessage, args);
	va_end(args);
#endif
}

//----------------------------------------------------------------------------------------------------

void Log::Write(LogType logType, const char* pMessage, va_list args)
{
#if defined(ENABLE_LOG)
	// Make sure we have initialized first
	if (!mInitialized)
	{
		MessageBoxA(nullptr, "Error: Log must be initialized first before writing any messages.", "Log", MB_OK | MB_ICONERROR);
		return;
	}

	// Open a file for writing
	FILE* pFile = nullptr;
	fopen_s(&pFile, mpFilename, "r+");

	// Check if we can open a file
	if (nullptr == pFile)
	{
		MessageBoxA(nullptr, "Error: Cannot open file for error log", "Log", MB_OK | MB_ICONERROR);
		return;
	}

	// Move file cursor to before the end of file body
	fseek(pFile, -14, SEEK_END);

	// Get message
	char msg[1024];
	vsprintf_s(msg, 1024, pMessage, args);

	// Get time
	char time[32];
	_strtime_s(time, 32);

	// Write the message and time into the file
	const char* pColour = c_LogMessageColour[static_cast<int>(logType)];
	fprintf_s(pFile, "<FONT COLOR = \"%s\">%s&nbsp;&nbsp;&nbsp;&nbsp;%s</FONT><BR>\n", pColour, time, msg);
	fprintf_s(pFile, "</BODY></HTML>");

	// Close the file
	fclose(pFile);
#endif
}