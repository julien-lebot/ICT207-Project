/*
*	Resource.hpp
*
*	An abstract resource type.
*	Inspired by Scott Bilas, Mark DeLoura; “Game Programming Gems - A Generic Handle-Based Resource Manager”
*	http://gyurchev.com/docs/resource2.doc
*
*	TODO: Create a Source object which will present a uniform interface between filesystem and virtual filesystem.
*
*  Created by Julien Lebot on 16/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Resource_hpp__
#define __Resource_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Atomic.hpp>
#include <Phoenix/Filesystem.hpp>
#include <string>

// Optional ?
//#include <boost/signals2/signal.hpp>

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
		/**
		 * Virtual destructor.
		 * Subclasses should deallocate resources in the unload method.
		 */
		virtual ~Resource();

		/**
		 * Constructor. Does not apply the RAII idiom since resource initialization can be very expensive; rely on prepare for that.
		 * @param creator The creator of that resource.
		 * @param name Specifies the name of the resource.
		 * @param handle Specifies the resource handle. Must be > 0.
		 */
		Resource(const ResourceManager* const creator, const std::string& name, const ResourceHandleType handle);

		/// Prepare the resource for loading.
		void prepare();

		/// Start the loading process for that resource
		void load();

		/// Reloads the resource
		void reload();

		/// Start the unloading process for that resource
		void unload();

		///Returns whether the Resource has been loaded or not.
		bool isLoaded() const 
		{ 
			return (mIsLoaded.get()); 
		}

		/// Sets the file path this resource will use.
#if defined(_USE_BOOST_)
		void setFilePath(const tr2::filesystem::path& path) { mFilePath = path; }
#endif
		/// Sets the file path this resource will use.
		void setFilePath(const std::string& path) { mFilePath = path; }

		/// Returns the handle associated with that resource.
		ResourceHandleType getHandle() const
		{
			return mHandle;
		}

		/// Returns the name associated with that resource.
		const std::string& getName() const
		{
			return mName;
		}

		/// Gets the manager which created this resource
		const ResourceManager* const getCreator(void)
		{
			return mResourceManager;
		}

		/// Public mutex for multithreading
		_AUTO_MUTEX;

		// Protected methods
	protected:
		/// Disable default construction
		Resource() 
		: mResourceManager(NULL),
		  mHandle(0),
		  mIsLoaded(false)
		{ 
		}

		/// Actual preparation implementation. Must be defined.
		virtual void prepareImpl() = 0;

		/// Actual loading implementation. Must be defined.
		virtual void loadImpl() = 0;

		/// Actual unloading implementation. Must be defined.
		virtual void unloadImpl() = 0;

		// Protected data members
	protected:
		/// Handle for quick lookups.0 designates an invalid handle, thus handles are >0.
		ResourceHandleType mHandle;

		/// The unique name of the resource.
		std::string mName;

		/// The creator of that resource
		const ResourceManager* mResourceManager;

		/// Is this resource loaded ?
		Atomic<bool> mIsLoaded;

		/// The file where this resource loads from. 
#if defined(_USE_BOOST_)
		tr2::filesystem::path mFilePath;
#else
		const std::string mFilePath;
#endif
	};

	typedef tr1::shared_ptr<Resource> ResourcePtr;
}

#endif // __Resource_hpp__
