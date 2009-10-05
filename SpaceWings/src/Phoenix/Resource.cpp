/*
*	Resource.cpp
*
*	An abstract resource type.
*	Inspired by Scott Bilas, Mark DeLoura; “Game Programming Gems - A Generic Handle-Based Resource Manager”
*	http://gyurchev.com/docs/resource2.doc
*
*  Created by Julien Lebot on 16/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/Resource.hpp>

using namespace Phoenix;

Resource::Resource(ResourceManager* creator, const std::string& name, const ResourceHandleType handle)
: mResourceManager(creator),
  mName(name),
  mHandle(handle),
  mLoadingState(UNLOADED)
{

}

Resource::~Resource()
{

}

void Resource::load()
{
	if (mLoadingState.get() != UNLOADED)
		return;
	else
		mLoadingState.set(LOADING);

	try
	{
		_LOCK_AUTO_MUTEX;
		if (mLoadingState.get() != UNLOADED)
			return;

		loadImpl();

		// Do something ? Like notify manager of resource creation success ?
		mLoadingState.set(LOADED);
	}
	catch (...)
	{
		mLoadingState.set(UNLOADED);
		throw;
	}
}

void Resource::reload()
{
	if (mLoadingState.get() != UNLOADED)
	{
		unload();
		load();
	}
}

void Resource::unload()
{
	if (mLoadingState.get() != LOADED)
		return;
	else
	{
		_LOCK_AUTO_MUTEX;
		if (mLoadingState.get() != LOADED)
			return;

		unloadImpl();
		mLoadingState.set(UNLOADED);
	}
}
