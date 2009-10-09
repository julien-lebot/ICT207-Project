/*
*	Resolution.hpp
*
*	A class representing the resolution of a screen
*
*  Created by Julien Lebot on 23/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Resolution_hpp__
#define __Resolution_hpp__

#include <Phoenix/Base.h>
#include <iostream>

namespace Phoenix
{
	template <typename T>
	class _PhoenixExport Resolution
	{
	public:
		/// Preferred constructor
		Resolution(const T& width, const T& height)
		: mWidth(width), mHeight(height)
		{}

		/// Copy constructor
		Resolution(const Resolution<T>& rhs)
		: mWidth(rhs.mWidth), mHeight(rhs.mHeight)
		{}

		/// Default constructor to allow storage in STL containers
		Resolution()
		{}

		/// Concrete destructor, do not subclass this class
		~Resolution() {}

		/// Assignment operator
		Resolution<T>& operator=(const Resolution<T>& rhs)
		{
			mWidth = rhs.mWidth;
			mHeight = rhs.mHeight;
			return *this;
		}

		/// Get the width
		const T& getWidth() const { return mWidth; }

		/// Get the height
		const T& getHeight() const { return mHeight; }

		friend std::ostream& operator <<(std::ostream& os, const Resolution<T>& res)
		{
			os << res.mWidth << "x" << res.mHeight;
			return os;
		}

	private:
		T mWidth, mHeight;
	};
	typedef Resolution<unsigned short> Resolution_us;
}

#endif // __Resolution_hpp__