#ifndef INCLUDED_RESOURCE_H
#define INCLUDED_RESOURCE_H

//====================================================================================================
// Filename:	Resource.h
// Description:	Base class for reference counted resource. Each resource is assigned a unique ID. To
//			  make use of the reference counting mechanism, the resource must be created/destroyed
//			  by ResourceManager, which is a friend class to Resource.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Utility/Key.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class Resource
{
	// We declare ResourceManager as a friend to grant it access to private functions in
	// the Resource class.
	friend class ResourceManager;

public:
	// Constructor
	Resource();

	// Destructor
	virtual ~Resource();

	// Functions to create/destroy the resource. All sub-classes need to specify how they should
	// be created and destroyed.
	virtual void Load(const char* pFilename) = 0;
	virtual void Unload() = 0;

	// Function to return the ID of this resource
	Utility::Key GetResourceID() const;

	// Function to return the reference count of the resource
	int GetRefCount() const;

private:
	// Function to assign an ID to this resource
	void SetResourceID(Utility::Key kID);

	// Function to increment the reference count of the resource. This is called whenever
	// ResourceManager returns a pointer to an instance of Resource.
	void IncrementRefCount();

	// Function to decrement the reference count of the resource. This is called whenever
	// ResourceManager tries to destroy a resource.
	void DecrementRefCount();

	// Members
	Utility::Key	mResourceID;   // "Unique" resource id generated from a string
	int			 mRefCount;	 // Reference count of the resource
};

#endif // #ifndef INCLUDED_RESOURCE_H