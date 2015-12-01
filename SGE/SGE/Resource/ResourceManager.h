#ifndef INCLUDED_RESOURCEMANAGER_H
#define INCLUDED_RESOURCEMANAGER_H

//====================================================================================================
// Filename:	ResourceManager.h
// Description:	Singleton class for managing an array of resource instances. It is responsible
//			  for creating/destroying new resource and make sure that only one instance of the
//			  resource is ever loaded. To create a new resource, call Create() with the resource
//			  class type as the template parameter. Resource created this way should be destroyed by
//			  calling Destroy(). The singleton can be accessed via a call to ResourceManager::Get().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <vector>

#include "Core/Log.h"
#include "Utility/Key.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Resource;

//====================================================================================================
// Class Declarations
//====================================================================================================

class ResourceManager
{
public:
	// Functions to create/destroy the singleton
	static void Create();
	static void Destroy();

	// Accessor function for singleton instance
	static ResourceManager* Get();

	// Destructor
	virtual ~ResourceManager();

	// Functions to startup and shutdown the resource manager
	void Initialize(int iPoolSize);
	void Terminate();

	// Function to "create" a new resource instance. It will search to see if the resource
	// has already been loaded. If so, it will increment the reference count of the loaded
	// resource by one and return the resource. If not, it will create a new instance and add
	// it to the resource list.
	template <typename T> T* Create(const char* pFilename);

	// Function to "destroy" a loaded resource instance. It will search to see if the resource
	// is added to the resource list. If so, it will decrement the reference count of the loaded
	// resource by one. If the reference count reaches zero, it will delete the resource and clear
	// it from the list.
	void Destroy(Resource* pResource);

	// Function to find a resource given its resource id
	template <typename T> T* Find(Utility::Key id);

protected:
	// Protected constructor for singleton
	ResourceManager();

private:
	static ResourceManager* s_pInstance;	   // Static instance for singleton

	std::vector<Resource*> mResourceList;   // Array of pointers to Resource

	bool mInitialized;						 // Init flag
};


//====================================================================================================
// Template Definitions
//====================================================================================================

template <typename T> T* ResourceManager::Create(const char* pFilename)
{
	// Check if the manager is initialized
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[ResourceManager] Failed to create resource. Manager not initialized.");
		return nullptr;
	}

	// Check if there is any space left
	if (mResourceList.size() == mResourceList.capacity())
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[ResourceManager] Failed to create resource. No more space!");
		return nullptr;
	}

	// Generate a key based on the filename
	Utility::Key kID = Utility::MakeKey(pFilename);

	// Check if this resource is already loaded
	const int kNumResources = mResourceList.size();
	for (int i = 0; i < kNumResources; ++i)
	{
		// If we find it, increment the ref count and return it
		if (mResourceList[i]->GetResourceID() == kID)
		{
			mResourceList[i]->IncrementRefCount();
			return dynamic_cast<T*>(mResourceList[i]);
		}
	}

	// Seems like it is not loaded yet, we will create a new instance...
	T* pNewResource = new T();
	pNewResource->SetResourceID(kID);
	pNewResource->Load(pFilename);

	// Add it to the loaded list and return it
	mResourceList.push_back(pNewResource);
	return pNewResource;
}

//----------------------------------------------------------------------------------------------------

template <typename T> T* ResourceManager::Find(Utility::Key id)
{
	// Check if the manager is initialized
	if (!mInitialized)
	{
		// Write to log
		Log::Get()->Write(LogType::Error, "[ResourceManager] Failed to find resource. Manager not initialized.");
		return nullptr;
	}

	// Find the resource
	const int kNumResources = mResourceList.size();
	for (unsigned int i = 0; i < kNumResources; ++i)
	{
		// If we find it, return the pointer to the resource
		if (mResourceList[i]->GetResourceID() == id)
		{
			return dynamic_cast<T*>(mResourceList[i]);
		}
	}

	return nullptr;
}

#endif // #ifndef INCLUDED_RESOURCEMANAGER_H