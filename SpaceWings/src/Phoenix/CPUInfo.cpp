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

#include <Phoenix/CPUInfo.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <sstream>
#include <string>
#include <iostream>

using namespace Phoenix;

struct CPUIDinfo
{ 
	unsigned __int32 EAX,EBX,ECX,EDX; 
};

/// SSE Features flag bits, found from Intel's doc, not sure if it works for all vendors.
enum SSEFlagBits
{
	SSE1 = (1<<25),
	SSE2 = (1<<26),
	SSE3 = 0x1,
	SSE4_1 = (1<<19),
	SSE4_2 = (1<<20)
};

#if (_ARCH_TYPE_ == _ARCH_64_) 
// This code is assembly for 64 bit target OS. 
// Assembly code must be compiled with the –use-msasm switch for Linux targets with the  
// Intel compiler.  
int isCPUIDsupported()
{ 
	// returns 1 if CPUID instruction supported on this processor, zero otherwise 
	// This isn't necessary on 64 bit processors because all 64 bit processor support CPUID 
	return 1; 
} 

void get_cpuid_info(CPUIDinfo *Info, const unsigned int leaf, const unsigned int subleaf) 
{ 
	// Stores CPUID return Info in the CPUIDinfo structure. 
	// leaf and subleaf used as parameters to the CPUID instruction 
	// parameters and register usage designed to be safe for both Windows and Linux 
	// Use the Intel compiler option -use-msasm when the target is Linux 
	__asm  
	{ 
		mov r10d, subleaf   ; arg2, subleaf (in R8 on WIN, in RDX on Linux) 
			mov r8, Info        ; arg0, array addr (in RCX on WIN, in RDI on Linux) 
			mov r9d, leaf       ; arg1, leaf (in RDX on WIN, in RSI on Linux) 
			push rax 
			push rbx 
			push rcx 
			push rdx 
			mov eax, r9d 
			mov ecx, r10d 
			cpuid 
			mov DWORD PTR [r8], eax 
			mov DWORD PTR [r8+4], ebx 
			mov DWORD PTR [r8+8], ecx 
			mov DWORD PTR [r8+12], edx 
			pop rdx 
			pop rcx 
			pop rbx 
			pop rax 
	} 
} 

#else // 32 bit 
//Note need to make sure -use-msasm switch is used with Intel compiler for Linux to get the 
// ASM code to compile for both windows and linux with one version source 

int isCPUIDsupported() 
{ 
	// returns 1 if CPUID instruction supported on this processor, zero otherwise 
	// This isn't necessary on 64 bit processors because all 64 bit Intel processors support CPUID 
	__asm  
	{ 
		push ecx ; save ecx 
			pushfd ; push original EFLAGS 
			pop eax ; get original EFLAGS 
			mov ecx, eax ; save original EFLAGS 
			xor eax, 200000h ; flip bit 21 in EFLAGS 
			push eax ; save new EFLAGS value on stack 
			popfd ; replace current EFLAGS value 
			pushfd ; get new EFLAGS 
			pop eax ; store new EFLAGS in EAX 
			xor eax, ecx ; Bit 21 of flags at 200000h will be 1 if CPUID exists 
			shr eax, 21  ; Shift bit 21 bit 0 and return it 
			push ecx 
			popfd ; restore bit 21 in EFLAGS first 
			pop ecx ; restore ecx 
	}
} 

//Note need to make sure -use-msasm switch is used with Intel compiler for Linux to get the 
// ASM code to compile for both windows and linux with one version source 
void get_cpuid_info (CPUIDinfo *Info, const unsigned int leaf, const unsigned int subleaf = 0x0) 
{ 
	// Stores CPUID return Info in the CPUIDinfo structure. 
	// leaf and subleaf used as parameters to the CPUID instruction 
	// parameters and registers usage designed to be safe for both Win and Linux 
	// when using -use-msasm 
	__asm  
	{ 
		mov edx, Info   ; addr of start of output array 
			mov eax, leaf  ; leaf 
			mov ecx, subleaf  ; subleaf 
			push edi 
			push ebx 
			mov  edi, edx                      ; edi has output addr 
			cpuid 
			mov DWORD PTR [edi], eax 
			mov DWORD PTR [edi+4], ebx 
			mov DWORD PTR [edi+8], ecx 
			mov DWORD PTR [edi+12], edx 
			pop ebx 
			pop edi 
	} 
} 
#endif

unsigned int getCPUFeatures()
{
	CPUIDinfo result;
	unsigned int features;

	// Check vendor strings
	if (memcmp(&result.EBX, "GenuineIntel", 12) == 0)
	{
		// Check standard feature
		get_cpuid_info(&result, 1);

		if (result.EDX & SSE1)
			features |= CPUInfo::SSE1;
		if (result.EDX & SSE2)
			features |= CPUInfo::SSE2;
		if (result.ECX & SSE3)
			features |= CPUInfo::SSE3;
		if (result.ECX & SSE4_1)
			features |= CPUInfo::SSE4_1;
		if (result.ECX & SSE4_2)
			features |= CPUInfo::SSE4_2;
	}
	else if (memcmp(&result.EBX, "AuthenticAMD", 12) == 0)
	{
		// Check standard feature
		get_cpuid_info(&result, 1);

		if (result.EDX & SSE1)
				features |= CPUInfo::SSE1;
		if (result.EDX & SSE2)
				features |= CPUInfo::SSE2;
		if (result.ECX & SSE3)
				features |= CPUInfo::SSE3;
	}

	return features;
}

std::string retrieveCPUString()
{
	CPUIDinfo result;
	char vendorString[0x20];
	char brandString[0x40];

	std::stringstream CPUStringStream;

	// Has standard feature ?
	get_cpuid_info(&result, 0);
	if (result.EAX)
	{
		memset(vendorString, 0, sizeof(vendorString));
		memset(brandString, 0, sizeof(brandString));

		*((int*)vendorString) = result.EBX;
		*((int*)(vendorString+4)) = result.EDX;
		*((int*)(vendorString+8)) = result.ECX;

		CPUStringStream << vendorString;

		// Calling get_cpuid_info with 0x80000000 as the query argument
		// gets the number of valid extended IDs.
		get_cpuid_info(&result, 0x80000000);
		unsigned int numExtensions = result.EAX;
		for (unsigned int i=0x80000000; i<=numExtensions; ++i)
		{
			get_cpuid_info(&result, i);

			// Interpret CPU brand string and cache information.
			if  (i == 0x80000002)
			{
				memcpy(brandString + 0, &result.EAX, sizeof(result.EAX));
				memcpy(brandString + 4, &result.EBX, sizeof(result.EBX));
				memcpy(brandString + 8, &result.ECX, sizeof(result.ECX));
				memcpy(brandString + 12, &result.EDX, sizeof(result.EDX));
			}
			else if  (i == 0x80000003)
			{
				memcpy(brandString + 16 + 0, &result.EAX, sizeof(result.EAX));
				memcpy(brandString + 16 + 4, &result.EBX, sizeof(result.EBX));
				memcpy(brandString + 16 + 8, &result.ECX, sizeof(result.ECX));
				memcpy(brandString + 16 + 12, &result.EDX, sizeof(result.EDX));
			}
			else if  (i == 0x80000004)
			{
				memcpy(brandString + 32 + 0, &result.EAX, sizeof(result.EAX));
				memcpy(brandString + 32 + 4, &result.EBX, sizeof(result.EBX));
				memcpy(brandString + 32 + 8, &result.ECX, sizeof(result.ECX));
				memcpy(brandString + 32 + 12, &result.EDX, sizeof(result.EDX));
			}
		}

		std::string brand(brandString);
		boost::trim(brand);

		if (!brand.empty())
			CPUStringStream << ": " << brand;

		return CPUStringStream.str();
	}

	return "X86";
}

std::string CPUInfo::featuresAsString()
{
	std::string featureStr;
	if (supportsFeature(SSE1))
		featureStr += "SSE ";
	if (supportsFeature(SSE2))
		featureStr += "SSE2 ";
	if (supportsFeature(SSE3))
		featureStr += "SSE3 ";
	if (supportsFeature(SSE4_1))
		featureStr += "SSE4.1 ";
	if (supportsFeature(SSE4_2))
		featureStr += "SSE4.2 ";
	return featureStr;
}

CPUInfo::CPUInfo()
{
	mSupportsCPUID = isCPUIDsupported() == 1 ? true : false;
	if (mSupportsCPUID)
	{
		mCPUString = retrieveCPUString();
		mCPUFeatures = getCPUFeatures();
	}
}

CPUInfo::~CPUInfo(){}