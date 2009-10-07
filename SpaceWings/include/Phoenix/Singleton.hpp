/*
*	Singleton.hpp
*
*	A singleton implementation based on Andrei Alexandrescu's Modern C++ Design Singleton
*
*  Created by Julien Lebot on 17/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Singleton_hpp__
#define __Singleton_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/NonCopyable.hpp>
#include <Phoenix/Empty.hpp>
#include <boost/thread.hpp>
#include <list>

namespace Phoenix
{
	/************************************************************************/
	/* Lifetime policies helper                                             */
	/************************************************************************/
	typedef void (_C_CALL_ *atexit_pfn_t)();

	namespace Details
	{
#ifndef _MAKE_DLL_
		void _C_CALL_ AtExitFn();  
#else
		void _PhoenixExport AtExitFn();
#endif

		class LifetimeTracker;

		// Helper data
		// std::list because of the insertion
		typedef std::list<LifetimeTracker*> TrackerArray;
		extern _PhoenixExport TrackerArray* pTrackerArray;

		/**
		 * Helper class for setLongevity
		 */
		class LifetimeTracker
		{
		public:
			LifetimeTracker(unsigned int x)
				: mLongevity(x) 
			{}

			virtual ~LifetimeTracker() = 0;

			static bool compare(const LifetimeTracker* lhs,
				const LifetimeTracker* rhs)
			{
				return lhs->mLongevity > rhs->mLongevity;
			}

		private:
			unsigned int mLongevity;
		};

		// Definition required
		inline LifetimeTracker::~LifetimeTracker() {} 

		// Helper destroyer function
		template <typename T>
		struct Deleter
		{
			typedef void (*Type)(T*);
			static void destroy(T* pObj)
			{
				delete pObj;
			}
		};

		// Concrete lifetime tracker for objects of type T
		template <typename T, typename Destroyer>
		class ConcreteLifetimeTracker : public LifetimeTracker
		{
		public:
			ConcreteLifetimeTracker(T* p,unsigned int longevity, Destroyer d)
				: LifetimeTracker(longevity),
				  mTracked(p),
				  mDestroyer(d)
			{}

			~ConcreteLifetimeTracker()
			{
				mDestroyer(mTracked);
			}

		private:
			T* mTracked;
			Destroyer mDestroyer;
		};

	} // namespace Private

	template <typename T, typename Destroyer>
	void setLongevity(T* pDynObject, unsigned int longevity, Destroyer d)
	{
		using namespace Details;

		TrackerArray pNewArray = static_cast<TrackerArray>(
			std::realloc(pTrackerArray, 
			sizeof(*pTrackerArray) * (elements + 1)));
		if (!pNewArray) throw std::bad_alloc();

		// Delayed assignment for exception safety
		pTrackerArray = pNewArray;

		LifetimeTracker* p = new ConcreteLifetimeTracker<T, Destroyer>(
			pDynObject, longevity, d);

		// Insert a pointer to the object into the queue
		TrackerArray pos = std::upper_bound(
			pTrackerArray, 
			pTrackerArray + elements, 
			p, 
			LifetimeTracker::compare);
		std::copy_backward(
			pos, 
			pTrackerArray + elements,
			pTrackerArray + elements + 1);
		*pos = p;
		++elements;

		// Register a call to AtExitFn
		std::atexit(Private::AtExitFn);
	}

	template <typename T>
	void setLongevity(T* pDynObject, unsigned int longevity,
		typename Details::Deleter<T>::Type d = Details::Deleter<T>::destroy)
	{
		setLongevity<T, typename Details::Deleter<T>::Type>(pDynObject, longevity, d);
	}

	/************************************************************************/
	/* Threading Policies                                                   */
	/************************************************************************/
	/**
	 * @param Single thread policy.
	 * Simply defines VolatileType as T.
	 */
	template <class T>
	class SingleThreadPolicy
	{
	public:
		typedef T VolatileType;
	};

	/**
	* @param Multi thread policy.
	* Defines VolatileType as volatile T so to avoid dangerous compiler optimizations.
	*/
	template <class T>
	class MultiThreadPolicy
	{
	public:
		typedef volatile T VolatileType;
	};

	/************************************************************************/
	/* Creation Policies                                                    */
	/************************************************************************/
	/**
	 * @brief Special creation policy that allows the client to manage the creation / destruction of the singleton themself.
	 */
	template <class T> struct CustomCreated
	{
		static T* create()
		{ return NULL; }

		static void destroy(T* p)
		{}
	};

	/**
	 * @brief Creation policy that uses new
	 */
	template <class T> struct CreateUsingNew
	{
		static T* create()
		{ return new T; }

		static void destroy(T* p)
		{ delete p; }
	};

	/**
	 * @brief Creation policy that uses a custom allocator.
	 */
	template<template<class> class Alloc>
	struct CreateUsing
	{
		template <class T>
		struct Allocator
		{
			static Alloc<T> allocator;

			static T* create()
			{
				return new (allocator.allocate(1)) T;
			}

			static void destroy(T* p)
			{
				//allocator.destroy(p);
				p->~T();
				allocator.deallocate(p,1);
			}
		};
	};

	/**
	* @brief Creation policy that uses a malloc.
	*/
	template <class T> struct CreateUsingMalloc
	{
		static T* create()
		{
			void* p = std::malloc(sizeof(T));
			if (!p) return 0;
			return new(p) T;
		}

		static void destroy(T* p)
		{
			p->~T();
			std::free(p);
		}
	};

	/************************************************************************/
	/* Lifetime policies                                                    */
	/************************************************************************/
	/**
	 * @brief A policy that follows C++ rules for deletion (LIFO).
	 */
	template <class T>
	struct DefaultLifetime
	{
		static void scheduleDestruction(T*, atexit_pfn_t pFun)
		{ std::atexit(pFun); }

		static void onDeadReference()
		{
			throw std::logic_error("Dead Reference Detected");
		}
	};

	/**
	 * Helper for SingletonWithLongevity
	 */
	namespace Details
	{
		template <class T>
		struct Adapter
		{
			void operator()(T*) { return pFun_(); }
			atexit_pfn_t pFun_;
		};
	}

	/**
	 * A Lifetime policy that allows client to specify the longevity of the singleton.
	 */
	template <class T>
	class SingletonWithLongevity
	{
	public:
		static void scheduleDestruction(T* pObj, atexit_pfn_t pFun)
		{
			Details::Adapter<T> adapter = { pFun };

			// getLongevity provided by clients
			setLongevity(pObj, getLongevity(pObj), adapter);
		}

		static void onDeadReference()
		{ throw std::logic_error("Dead Reference Detected"); }
	};

	/**
	 * Invincible singleton that never dies.
	 */
	template <class T>
	struct NoDestroy
	{
		static void scheduleDestruction(T*, atexit_pfn_t)
		{}

		static void onDeadReference()
		{}
	};

	/**
	 * @brief A singleton implementation based on Andrei Alexandrescu's singleton.
	 * @bug: The threading policy does not really work.
	 */
	template
	<
		class T,
		template <class> class CreationPolicy = CreateUsingNew,
		template <class> class LifetimePolicy = DefaultLifetime/*,
		template <class> class ThreadingModel = SingleThreadPolicy*/
	>
	class Singleton
		: private NonCopyable
	{
	public:
		_STATIC_MUTEX(singletonMutex);

		static T& instance()
		{

			if (!mInstance)
			{
				_LOCK_MUTEX(singletonMutex);
				if (!mInstance)
				{
					if (mDestroyed)
					{
						LifetimePolicy<T>::onDeadReference();
						mDestroyed = false;
					}

					mInstance = CreationPolicy<T>::create();
					LifetimePolicy<T>::scheduleDestruction(mInstance, &destroySingleton);
				}
			}
			return *mInstance;
		}
	private:
		static void destroySingleton()
		{
			assert(!mDestroyed);
			CreationPolicy<T>::destroy(mInstance);
			mInstance = 0;
			mDestroyed = true;
		}
		//typedef typename ThreadingModel<T>::VolatileType InstanceType;
		static T *mInstance;
		static bool mDestroyed;
	};

	/**
	 * @brief Partial template specialization for the special case where the type the singleton applies to is abstract and can't be directly
	 * instantiated. In that special case, the client takes care of creating the singleton and the latter initialize itself during construction.
	 * In that very special case the client code takes care of the creation and destruction of the singleton entirely.
	 */
	template
	<
		class T,
		template <class> class LifetimePolicy
	>
	class Singleton<T, CustomCreated, LifetimePolicy>
		: private NonCopyable
	{
	public:
		_STATIC_MUTEX(singletonMutex);

		Singleton()
		{
			mInstance = static_cast<T*>(this);
		}

		static T& instance()
		{
			if (!mInstance)
			{
				_LOCK_MUTEX(singletonMutex);
				if (!mInstance)
				{
					if (mDestroyed)
					{
						LifetimePolicy<T>::onDeadReference();
						mDestroyed = false;
					}
					LifetimePolicy<T>::scheduleDestruction(mInstance, &destroySingleton);
				}
			}
			return *mInstance;
		}
	private:
		static void destroySingleton()
		{
			assert(!mDestroyed);
			mInstance = 0;
			mDestroyed = true;
		}
		static T *mInstance;
		static bool mDestroyed;
	};
}

#endif // __Singleton_hpp__