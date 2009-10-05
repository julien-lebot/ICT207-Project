/*
*	Resource.hpp
*
*	An abstract resource type.
*	Inspired by Scott Bilas, Mark DeLoura; “Game Programming Gems - A Generic Handle-Based Resource Manager”
*	http://gyurchev.com/docs/resource2.doc
*
*  Created by Julien Lebot on 16/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Resource_hpp__
#define __Resource_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Atomic.hpp>
#include <string>

// #include "ResourceManager.h"

namespace Phoenix
{
	// Fwd declaration
	class ResourceManager;

	/// The handle type
	typedef unsigned int ResourceHandleType;

	class Resource
	{
	public:

		/// Loading state of the resource
		enum LoadingState
		{
			/// Not loaded
			UNLOADED,
			/// Loading is in progress
			LOADING,
			/// Fully loaded
			LOADED,
			/// Currently unloading
			UNLOADING
		};

		/**
		 * Virtual destructor.
		 * Subclasses should deallocate resources in the unload method.
		 */
		virtual ~Resource();

		/**
		 * Constructor.
		 * @param creator The creator of that resource.
		 * @param name Specifies the name of the resource.
		 * @param handle Specifies the resource handle. Must be > 0.
		 */
		Resource(ResourceManager* creator, const std::string& name, const ResourceHandleType handle);

		/// Start the loading process for that resource
		virtual void load();

		/// Reloads the resource
		virtual void reload();

		/// Start the unloading process for that resource
		virtual void unload();

		///Returns whether the Resource has been loaded or not.
		virtual bool isLoaded() const 
		{ 
			return (mLoadingState.get() == LOADED); 
		}

		/**
		 * Returns whether the resource is currently in the process of loading.
		 */
		virtual bool isLoading() const
		{
			return (mLoadingState.get() == LOADING);
		}

		/// Returns the current loading state.
		virtual LoadingState getLoadingState() const
		{
			return mLoadingState.get();
		}

		/// Returns the handle associated with that resource.
		virtual ResourceHandleType getHandle() const
		{
			return mHandle;
		}

		/// Returns the name associated with that resource.
		virtual const std::string& getName() const
		{
			return mName;
		}

		/// Gets the manager which created this resource
		virtual ResourceManager* getCreator(void)
		{
			return mResourceManager;
		}

		/// Public mutex for multithreading
		_AUTO_MUTEX;

	protected:
		/// Disable default construction
		Resource() 
		: mResourceManager(NULL),
		  mHandle(0),
		  mLoadingState(UNLOADED)
		{ 
		}

		virtual void loadImpl() = 0;
		virtual void unloadImpl() = 0;

	protected:
		/// Handle for quick lookups.0 designates an invalid handle, thus handles are >0.
		ResourceHandleType mHandle;

		/// The unique name of the resource.
		std::string mName;

		/// The creator of that resource
		ResourceManager* mResourceManager;

		/// Tracks the state of the loading process for that resource. It is atomic for multithreading safety.
		Atomic<LoadingState> mLoadingState;
	};

	typedef tr1::shared_ptr<Resource> ResourcePtr;
}

#endif // __Resource_hpp__
