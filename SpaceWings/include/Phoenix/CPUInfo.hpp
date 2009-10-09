/*
*	CPUInfo.hpp
*
*	A class to obtain CPU informations.
*	Most of the code here has been obtained through various sources on the internet, the most important one being:
*	http://software.intel.com/en-us/articles/using-cpuid-to-detect-the-presence-of-sse-41-and-sse-42-instruction-sets/
*   Original code written by Patrick Fay, Ronen Zohar and Shihjong Kuo . 
*	Modified by Garrett Drysdale for the mentioned application note.
*
*	http://www.intel.com/Assets/PDF/appnote/241618.pdf
*
*  Created by Julien Lebot on 08/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __CPUInfo_hpp__
#define __CPUInfo_hpp__

#include <Phoenix/Base.h>

namespace Phoenix
{
	class CPUInfo
	{
	public:
		enum CpuFeatures
		{
			SSE1	= 1 << 0,
			SSE2	= 1 << 1,
			SSE3	= 1 << 2,
			SSE4_1	= 1 << 3,
			SSE4_2	= 1 << 4
		};

		CPUInfo();
		~CPUInfo();
		std::string& getCPUString() { return mCPUString; }
		bool supportsCPUID() { return mSupportsCPUID; }
		bool supportsFeature(const CpuFeatures featureFlag) { return (mCPUFeatures & featureFlag) != 0; }
		std::string featuresAsString();
	protected:
		bool mSupportsCPUID;
		std::string mCPUString;
		unsigned int mCPUFeatures;
	};
}

#endif // __CPUInfo_hpp__