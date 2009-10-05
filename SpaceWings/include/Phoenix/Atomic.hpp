/*
*	Atomic.hpp
*
*	A template class for atomic operation similar to what Objective-C properties have.
*
*  Created by Julien Lebot on 16/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Atomic_hpp__
#define __Atomic_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Empty.hpp>
#include <boost/thread.hpp>

namespace Phoenix
{
	template
		<
			typename T,
			bool AtomicGetter = true
		>
	class Atomic
	{
	public:
		Atomic(const T& default)
		: mField(default)
		{}

		Atomic(const Atomic<T>& copy)
		: mField(copy.get())
		{}

		Atomic()
		{}

		void operator= (const Atomic<T> &rhs)
		{
			set(rhs.get());
		}

		T get() const
		{
			_LOCK_AUTO_MUTEX;
			return mField;
		}

		void set(const T& val)
		{
			_LOCK_AUTO_MUTEX;
			mField = val;
		}

	protected:
		_AUTO_MUTEX;

		volatile T mField;
	};

	template
		<
			typename T
		>
	class Atomic<T, false>
	{
	public:
		Atomic(const T& default)
		: mField(default)
		{}

		Atomic(const Atomic<T, false>& copy)
		: mField(copy.get())
		{}

		Atomic()
		{}

		void operator= (const Atomic<T, false> &rhs)
		{
			set(rhs.get());
		}

		T get() const
		{
			return mField;
		}

		void set(const T& val)
		{
			_LOCK_AUTO_MUTEX;
			mField = val;
		}

	protected:
		_AUTO_MUTEX;

		volatile T mField;
	};
}

#endif // __Atomic_hpp__