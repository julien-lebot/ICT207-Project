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

#ifndef __ResourceManager_hpp__
#define __ResourceManager_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Resource.hpp>
#include <Phoenix/NonCopyable.hpp>
#include <hash_map>
#include <stack>

namespace Phoenix
{
	/**
	 * @brief A class for managing resource handles.
	 *
	 * @param ResourceHandleType The resource handle type: must be modeled after an integer-compatible type e.g. it must at least:
	 *	- store numerals
	 *	- have ++ prefix and or postfix operators
	 *	- have operator=
	 *	- be copy constructible
	 *	- be default constructible
	 */
	template <typename ResourceHandleType>
	class ResourceHandleCreator
		: private NonCopyable
	{
	public:
		typedef std::list<ResourceHandleType> FreeHandleListType;

		/// Default constructor
		ResourceHandleCreator()
		: mNextHandle(0)
		{}

		/// Don't need a virtual destructor since this class is concrete. Do NOT inherit from it or inherit privately.
		~ResourceHandleCreator() {}	// TODO: check if free store is empty

		/// Creates a new handle or reuse a free one
		ResourceHandleType createHandle()
		{
			ResourceHandleType retVal;

			// Free store is empty, creates new handles
			if (mFreeHandles.empty())
				retVal = ++mNextHandle;
			else
			{
				// Get last inserted element
				retVal = mFreeHandles.back();

				// Erase last element from free store
				mFreeHandles.pop_back();
			}

			return retVal;
		}

		/// Delete the specified handle. Warning, might be slow.
		void deleteHandle(ResourceHandleType handle)
		{
			// Validate handle first
			if (isValid(handle))
				mFreeHandles.push_back(handle);
		}

		/// Purges all the resource handles, like a reset. Use with care ! It will invalidate all the handles !
		void purge()
		{
			mNextHandle = 0;
			mFreeHandles.clear();
		}

		/// Check if the handle is valid. Warning, might be slow
		bool isValid(const ResourceHandleType handle)
		{
			// Handle can't be greater to what was already allocated, nor can it be <= 0
			if (handle >= mNextHandle || handle <= 0)
				return false;
			else
			{
				// Search the free store
				for (FreeHandleListType::const_iterator cit = mFreeHandles.begin(); cit != mFreeHandles.end(); ++cit)
				{
					// This handle has been previously deallocated
					if (handle == *cit)
						return false;
				}
			}
			return true;
		}

	protected:
		FreeHandleListType mFreeHandles;
		ResourceHandleType mNextHandle;
	};

	class ResourceManager
	: private NonCopyable
	{
	public:
		/// Mutex for object-wise locking
		_AUTO_MUTEX;

		/// Default constructor
		ResourceManager();

		/// Virtual Destructor
		virtual ~ResourceManager();

		/// Use a hash map when dealing with resources' name as it is faster
		typedef stdext::hash_map<std::string, ResourcePtr> ResourceMapType;

		/// Use a std::map when dealing with resources' handles as it organizes them on the go.
		typedef std::map<ResourceHandleType, ResourcePtr> ResourceHandleMapType;

		virtual std::pair<ResourcePtr, bool> createGet(const std::string& name);

		/**
		 * Creates a blank resource but does not load it.<br/>
		 * If a resource of the same name exist it will return a pointer to it.
		 * @param name The name of the resource to create
		 */
		virtual ResourcePtr create(const std::string& name);

		virtual ResourcePtr get(const std::string& name) const;

		virtual ResourcePtr get(const ResourceHandleType handle) const;

		virtual ResourcePtr load(const std::string& name);

		/**
		 * Unloads a resource; the resource is not necessarily destroyed and can be reloaded if necessary.
		 * @param name The name of the resource to unload.
		 */
		virtual void unload(const std::string& name);

		/**
		* Unloads a resource; the resource is not necessarily destroyed and can be reloaded if necessary.
		* @param handle The handle to the resource to unload.
		*/
		virtual void unload(const ResourceHandleType handle);

		/// Unload all resources.
		virtual void unloadAll();

		virtual void remove(const ResourcePtr& res);

		virtual void remove(const std::string& name);

		virtual void remove(const ResourceHandleType handle);

		virtual void removeAll();

		virtual void reloadAll();

	protected:
		virtual ResourcePtr createImpl(const std::string& name, const ResourceHandleType handle) = 0;
		virtual void addImpl(const ResourcePtr& res);
		virtual void removeImpl(const ResourcePtr& res);

	protected:
		ResourceMapType mResourceMap;
		ResourceHandleMapType mResourceHandleMap;
		ResourceHandleCreator<ResourceHandleType> mResourceHandleCreator;
	};
}

#endif // __ResourceManager_hpp__
