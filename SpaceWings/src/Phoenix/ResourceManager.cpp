/*
*	ResourceManager.hpp
*
*	An abstract resource manager.
*	Inspired by Scott Bilas, Mark DeLoura; “Game Programming Gems - A Generic Handle-Based Resource Manager”
*	http://gyurchev.com/docs/resource2.doc
*
*  Created by Julien Lebot on 16/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/ResourceManager.hpp>

using namespace Phoenix;

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	removeAll();
}

std::pair<ResourcePtr, bool> ResourceManager::createGet(const std::string& name)
{
	_LOCK_AUTO_MUTEX;

	ResourcePtr retVal = get(name);

	if (retVal.get() == NULL)
		return std::pair<ResourcePtr, bool>(create(name), true);
	else
		return std::pair<ResourcePtr, bool>(retVal, false);
}

ResourcePtr ResourceManager::create(const std::string& name)
{
	// Call the implementation to create the resource
	ResourcePtr retVal = ResourcePtr(createImpl(name, mResourceHandleCreator.createHandle()));

	// Call the implementation to add the resource to the manager
	addImpl(retVal);

	return retVal;
}

ResourcePtr ResourceManager::get(const std::string& name) const
{
	_LOCK_AUTO_MUTEX;

	ResourceMapType::const_iterator cit = mResourceMap.find(name);
	if (cit != mResourceMap.end())
		return cit->second;		// Return mapped type if found
	else
		return ResourcePtr();	// Return NULL pointer if not found
}

ResourcePtr ResourceManager::get(const ResourceHandleType handle) const
{
	_LOCK_AUTO_MUTEX;

	ResourceHandleMapType::const_iterator cit = mResourceHandleMap.find(handle);
	if (cit != mResourceHandleMap.end())
		return cit->second;		// Return mapped type if found
	else
		return ResourcePtr();	// Return NULL pointer if not found
}

ResourcePtr ResourceManager::load(const std::string& name)
{
	ResourcePtr retVal = get(name);

	if (retVal.get() != NULL)
		retVal->load();

	return retVal;
}

void ResourceManager::unload(const std::string& name)
{
	ResourcePtr res = get(name);

	if (res.get() != NULL)
		res->unload();
}
void ResourceManager::unload(const ResourceHandleType handle)
{
	ResourcePtr res = get(handle);

	if (res.get() != NULL)
		res->unload();
}

void ResourceManager::unloadAll()
{
	_LOCK_AUTO_MUTEX;

	ResourceMapType::iterator it;
	for (it = mResourceMap.begin(); it != mResourceMap.end(); ++it)
	{
		it->second->unload();
	}
}

void ResourceManager::remove(const ResourcePtr& res)
{
	removeImpl(res);
}

void ResourceManager::remove(const std::string& name)
{
	ResourcePtr res = get(name);

	if (res.get() != NULL)
	{
		removeImpl(res);
	}
}

void ResourceManager::remove(const ResourceHandleType handle)
{
	ResourcePtr res = get(handle);

	if (res.get() != NULL)
	{
		removeImpl(res);
	}
}

void ResourceManager::removeAll()
{
	_LOCK_AUTO_MUTEX;

	mResourceMap.clear();
	mResourceHandleMap.clear();
}

void ResourceManager::reloadAll()
{
	_LOCK_AUTO_MUTEX;

	ResourceMapType::iterator it;
	for (it = mResourceMap.begin(); it != mResourceMap.end(); ++it)
	{
		it->second->reload();
	}
}

void ResourceManager::addImpl(const ResourcePtr& res)
{
	_LOCK_AUTO_MUTEX;

	std::pair<ResourceMapType::iterator, bool> result = mResourceMap.insert(ResourceMapType::value_type(res->getName(), res));
	if (!result.second)
	{
		std::cout << "Error dup" << std::endl;
		// TODO: throw exception
	}
	else
	{
		std::pair<ResourceHandleMapType::iterator, bool> resultHandle = mResourceHandleMap.insert(ResourceHandleMapType::value_type(res->getHandle(), res));
		if (!resultHandle.second)
		{
			std::cout << "Error dup" << std::endl;
			// TODO: throw exception
		}
	}
}

void ResourceManager::removeImpl(const ResourcePtr& res)
{
	_LOCK_AUTO_MUTEX;

	ResourceMapType::iterator it = mResourceMap.find(res->getName());
	if (it != mResourceMap.end())
	{
		mResourceMap.erase(it);

		ResourceHandleMapType::iterator itHandle = mResourceHandleMap.find(res->getHandle());
		if (itHandle != mResourceHandleMap.end())
		{
			// Return the handle to the free store
			mResourceHandleCreator.deleteHandle(itHandle->first);

			// Erase from the map
			mResourceHandleMap.erase(itHandle);
		}
	}
	// Throw exception if not found ?
}