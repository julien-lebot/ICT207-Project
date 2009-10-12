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
*	See also http://www.lpjjl.net/pgm/fichiers/intelcountingprocessors.htm
*
*  Created by Julien Lebot on 08/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __CPUInfo_hpp__
#define __CPUInfo_hpp__

#include <Phoenix/Base.h>
#include <vector>

namespace Phoenix
{
	class CPUInfo
	{
	public:
		/// Enumeration of all known CPU features
		enum CpuFeatures
		{
			SSE3         = 0, 
			PCLMUL       = 1,
			DTES64       = 2,
			MONITOR      = 3,  
			DS_CPL       = 4,  
			VMX          = 5,  
			SMX          = 6,  
			EST          = 7,  
			TM2          = 8,  
			SSSE3        = 9,  
			CID          = 10,
			FMA          = 12,
			CX16         = 13, 
			ETPRD        = 14, 
			PDCM         = 15, 
			DCA          = 18, 
			SSE4_1       = 19, 
			SSE4_2       = 20, 
			x2APIC       = 21, 
			MOVBE        = 22, 
			POPCNT       = 23, 
			XSAVE        = 26, 
			OSXSAVE      = 27, 
			AVX          = 28,

			FPU          = 29,  
			VME          = 30,  
			DE           = 31,  
			PSE          = 32,  
			TSC          = 33,  
			MSR          = 34,  
			PAE          = 35,  
			MCE          = 36,  
			CX8          = 37,  
			APIC         = 38,  
			SEP          = 40, 
			MTRR         = 41, 
			PGE          = 42, 
			MCA          = 43, 
			CMOV         = 44, 
			PAT          = 45, 
			PSE36        = 46, 
			PSN          = 47, 
			CLF          = 48, 
			DTES         = 50, 
			ACPI         = 51, 
			MMX          = 52, 
			FXSR         = 53, 
			SSE          = 54, 
			SSE2         = 55, 
			SS           = 56, 
			HTT          = 57, 
			TM1          = 58, 
			IA64         = 59,
			PBE          = 60
		};

		/// Default constructor
		CPUInfo();

		/// Concrete destructor
		~CPUInfo();

		/// Returns a string describing the CPU
		std::string& getCPUString() { return mCPUString; }

		/// Determines whether the CPU supports the CPUID instruction
		bool supportsCPUID() { return mSupportsCPUID; }

		/// Determines if the CPU supports a specific feature
		bool supportsFeature(const CpuFeatures featureBit) { return (mCPUFeatures & (1i64 << featureBit)) != 0; }

		/// Get all the supported features as a vector of strings
		std::vector<std::string> featuresAsVector();

		/// Get all the supported features as a single string with each value delimited by the specified delimiter
		std::string CPUInfo::featuresAsString(const char delimiter = ' ');

		/// Get the number of logical CPU per package.
		unsigned short getNumLogicalCPUs() { return mNumLogicalCPUs; }
	protected:
		bool mSupportsCPUID;
		std::string mCPUString;
		unsigned long long mCPUFeatures;
		unsigned short mNumLogicalCPUs;
	};
}

#endif // __CPUInfo_hpp__