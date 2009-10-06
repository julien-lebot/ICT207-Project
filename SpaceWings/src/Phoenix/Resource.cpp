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

Resource::Resource(const ResourceManager* const creator,
				   const std::string& name,
				   const ResourceHandleType handle)
: mResourceManager(creator),
  mName(name),
  mHandle(handle),
  mIsLoaded(false)
{

}

Resource::~Resource()
{

}

void Resource::prepare()
{
	{
		_LOCK_AUTO_MUTEX;
		prepareImpl();
	}
}

void Resource::load()
{
	if (mIsLoaded.get())
		return;

	try
	{
		_LOCK_AUTO_MUTEX;
		if (mIsLoaded.get())
			return;

		loadImpl();

		// Do something ? Like notify manager of resource creation success ?
		mIsLoaded.set(true);
	}
	catch (...)
	{
		mIsLoaded.set(false);
		throw;
	}
}

void Resource::reload()
{
	if (mIsLoaded.get())
	{
		unload();
		load();
	}
}

void Resource::unload()
{
	if (!mIsLoaded.get())
		return;
	else
	{
		_LOCK_AUTO_MUTEX;
		if (!mIsLoaded.get())
			return;

		unloadImpl();
		mIsLoaded.set(false);
	}
}
