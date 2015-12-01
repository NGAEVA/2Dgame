 //===================================================================================================
// Filename:	ResourceManager.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Resource/ResourceManager.h"

#include <assert.h>

#include "Resource/Resource.h"

//====================================================================================================
// Statics
//====================================================================================================

ResourceManager* ResourceManager::s_pInstance = nullptr;

//====================================================================================================
// Class Definitions
//====================================================================================================

void ResourceManager::Create()
{
	assert(s_pInstance == nullptr);
	s_pInstance = new ResourceManager();
}

//----------------------------------------------------------------------------------------------------

void ResourceManager::Destroy()
{
	assert(s_pInstance != nullptr);
	delete s_pInstance;
	s_pInstance = nullptr;
}

//----------------------------------------------------------------------------------------------------

ResourceManager* ResourceManager::Get()
{
	assert(s_pInstance != nullptr);
	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

ResourceManager::ResourceManager() :
	mInitialized(false)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

ResourceManager::~ResourceManager()
{
	// Terminate the resource manager
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void ResourceManager::Initialize(int iPoolSize)
{
	// Check if we have already initialized the manager
	if (mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[ResourceManager] Manager already initialized.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[ResourceManager] Initializing...");

	// Initialize resource pool size
	mResourceList.reserve(iPoolSize);

	// Set flag
	mInitialized = true;

	// Write to log
	Log::Get()->Write(LogType::Message, "[ResourceManager] Manager initialized.");
}

//----------------------------------------------------------------------------------------------------

void ResourceManager::Terminate()
{
	// Check if we have already terminated the manager
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Warning, "[ResourceManager] Manager already terminated.");
		return;
	}

	// Write to log
	Log::Get()->Write(LogType::Message, "[ResourceManager] Terminating...");

	// Clear the list
	const int kNumResources = mResourceList.size();
	for (int i = 0; i < kNumResources; ++i)
	{
		if (nullptr != mResourceList[i])
		{
			delete mResourceList[i];
			mResourceList[i] = nullptr;
		}
	}
	mResourceList.clear();

	// Reset flag
	mInitialized = false;

	// Write to log
	Log::Get()->Write(LogType::Message, "[ResourceManager] Manager terminated");
}

//----------------------------------------------------------------------------------------------------

void ResourceManager::Destroy(Resource* pResource)
{
	// Check if the manager is initialized
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[ResourceManager] Failed to destroy resource. Manager not initialized.");
		return;
	}

	// Get the resource ID
	Utility::Key kID = pResource->GetResourceID();

	// Check if this resource is in the loaded list
	for (std::vector<Resource*>::iterator itr = mResourceList.begin();
		 itr != mResourceList.end();
		 ++itr)
	{
		// If we find it, decrement the ref count
		if (nullptr != (*itr) && kID == (*itr)->GetResourceID())
		{
			(*itr)->DecrementRefCount();
			
			// If there are no more reference to it, then we will delete it
			if ((*itr)->GetRefCount() <= 0)
			{
				delete (*itr);
				mResourceList.erase(itr);
				return;
			}
		}
	}
}
