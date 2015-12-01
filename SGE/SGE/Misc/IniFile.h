#ifndef INCLUDED_INIFILE_H
#define INCLUDED_INIFILE_H

//====================================================================================================
// Filename:	IniFile.h
// Description:	Singleton class for the init file. It provides an interface for data query via string
//			  tags. Internally it contains three stl maps of string/value pairs: str/float,
//			  str/bool and str/str. The singleton can be accessed via a call to IniFile::Get().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <map>
#include <string>

//====================================================================================================
// Class Declarations
//====================================================================================================

class IniFile
{
public:
	// Functions to create/destroy the singleton
	static void Create();
	static void Destroy();

	// Accessor function for singleton instance
	static IniFile* Get();

	// Destructor
	virtual ~IniFile();

	// Functions to Load/Unload the init file
	void Load(const char* pFilename);
	void Unload();

	// Functions to get data entry by string tag
	int GetInt(const char* pTag, int iDefault = 0) const;
	bool GetBool(const char* pTag, bool bDefault = false) const;
	float GetFloat(const char* pTag, float fDefault = 0.0f) const;
	const char* GetString(const char* pTag, const char* pDefault = "") const;

protected:
	// Protected constructor for singleton
	IniFile();

private:
	// Typedefs
	typedef std::map<std::string, bool> BoolTable;
	typedef std::map<std::string, float> FloatTable;
	typedef std::map<std::string, std::string> StringTable;

	// Members
	static IniFile* s_pInstance;   // Static instance for singleton

	BoolTable mBoolEntries;		// A hash table of string/bool pairs
	FloatTable mFloatEntries;	  // A hash table of string/float pairs
	StringTable mStringEntries;	// A hash table of string/string pairs

	bool mLoaded;				  // Loaded flag
};

#endif // #ifndef INCLUDED_INIFILE_H