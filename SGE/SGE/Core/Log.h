#ifndef INCLUDED_LOG_H
#define INCLUDED_LOG_H

//====================================================================================================
// Filename:	Log.h
// Description:	Singleton class for logging messages. There are three log message types: standard
//				messages (white), warnings (yellow), and errors (red). The messages are written to an
//				external HTML file. The singleton can be accessed via a call to Log::Get().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#define WIN32_LEAN_AND_MEAN	// Reduce windows include scope
#include <windows.h>

//====================================================================================================
// Enums
//====================================================================================================

enum class LogType
{
	Message,
	Warning,
	Error,
	MAX
};

//====================================================================================================
// Class Declarations
//====================================================================================================

class Log
{
public:
	// Functions to create/destroy the singleton
	static void Create();
	static void Destroy();

	// Accessor function for singleton instance
	static Log* Get();

public:
	// Initialize the log
	void Initialize(const char* pFilename = "Log.html");

	// Terminate the log
	void Terminate();

	// Write to log
	void Write(LogType logType, const char* pMessage, ...);
	void Write(LogType logType, const char* pMessage, va_list args);

protected:
	// Protected constructor for singleton
	Log();

private:
	static Log* s_pInstance;	// Static instance for singleton

	char* mpFilename;			// Name of log file
	bool mInitialized;			// Init flag
};

#endif // #ifndef INCLUDED_LOG_H