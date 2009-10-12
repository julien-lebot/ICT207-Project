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
	typedef unsigned __int32 RegisterType;
	RegisterType EAX,EBX,ECX,EDX; 
};

/// http://wiki.osdev.org/CPUID
enum CPUIDFlags {
	CPUID_FEAT_ECX_SSE3         = 1 << 0, 
	CPUID_FEAT_ECX_PCLMUL       = 1 << 1,
	CPUID_FEAT_ECX_DTES64       = 1 << 2,
	CPUID_FEAT_ECX_MONITOR      = 1 << 3,  
	CPUID_FEAT_ECX_DS_CPL       = 1 << 4,  
	CPUID_FEAT_ECX_VMX          = 1 << 5,  
	CPUID_FEAT_ECX_SMX          = 1 << 6,  
	CPUID_FEAT_ECX_EST          = 1 << 7,  
	CPUID_FEAT_ECX_TM2          = 1 << 8,  
	CPUID_FEAT_ECX_SSSE3        = 1 << 9,  
	CPUID_FEAT_ECX_CID          = 1 << 10,
	CPUID_FEAT_ECX_FMA          = 1 << 12,
	CPUID_FEAT_ECX_CX16         = 1 << 13, 
	CPUID_FEAT_ECX_ETPRD        = 1 << 14, 
	CPUID_FEAT_ECX_PDCM         = 1 << 15, 
	CPUID_FEAT_ECX_DCA          = 1 << 18, 
	CPUID_FEAT_ECX_SSE4_1       = 1 << 19, 
	CPUID_FEAT_ECX_SSE4_2       = 1 << 20, 
	CPUID_FEAT_ECX_x2APIC       = 1 << 21, 
	CPUID_FEAT_ECX_MOVBE        = 1 << 22, 
	CPUID_FEAT_ECX_POPCNT       = 1 << 23, 
	CPUID_FEAT_ECX_XSAVE        = 1 << 26, 
	CPUID_FEAT_ECX_OSXSAVE      = 1 << 27, 
	CPUID_FEAT_ECX_AVX          = 1 << 28,

	CPUID_FEAT_EDX_FPU          = 1 << 0,  
	CPUID_FEAT_EDX_VME          = 1 << 1,  
	CPUID_FEAT_EDX_DE           = 1 << 2,  
	CPUID_FEAT_EDX_PSE          = 1 << 3,  
	CPUID_FEAT_EDX_TSC          = 1 << 4,  
	CPUID_FEAT_EDX_MSR          = 1 << 5,  
	CPUID_FEAT_EDX_PAE          = 1 << 6,  
	CPUID_FEAT_EDX_MCE          = 1 << 7,  
	CPUID_FEAT_EDX_CX8          = 1 << 8,  
	CPUID_FEAT_EDX_APIC         = 1 << 9,  
	CPUID_FEAT_EDX_SEP          = 1 << 11, 
	CPUID_FEAT_EDX_MTRR         = 1 << 12, 
	CPUID_FEAT_EDX_PGE          = 1 << 13, 
	CPUID_FEAT_EDX_MCA          = 1 << 14, 
	CPUID_FEAT_EDX_CMOV         = 1 << 15, 
	CPUID_FEAT_EDX_PAT          = 1 << 16, 
	CPUID_FEAT_EDX_PSE36        = 1 << 17, 
	CPUID_FEAT_EDX_PSN          = 1 << 18, 
	CPUID_FEAT_EDX_CLF          = 1 << 19, 
	CPUID_FEAT_EDX_DTES         = 1 << 21, 
	CPUID_FEAT_EDX_ACPI         = 1 << 22, 
	CPUID_FEAT_EDX_MMX          = 1 << 23, 
	CPUID_FEAT_EDX_FXSR         = 1 << 24, 
	CPUID_FEAT_EDX_SSE          = 1 << 25, 
	CPUID_FEAT_EDX_SSE2         = 1 << 26, 
	CPUID_FEAT_EDX_SS           = 1 << 27, 
	CPUID_FEAT_EDX_HTT          = 1 << 28, 
	CPUID_FEAT_EDX_TM1          = 1 << 29, 
	CPUID_FEAT_EDX_IA64         = 1 << 30,
	CPUID_FEAT_EDX_PBE          = 1 << 31
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

unsigned long long getCPUFeatures()
{
	CPUIDinfo result;
	unsigned long long features = 0;

	get_cpuid_info(&result, 1);

	for (unsigned int i = 0; i < CPUInfo::PBE; i++)
	{
		if (i <= 28)
		{
			if (result.ECX & (1i64 << i))
			{
				if (i <= 15)
				{
					features |= (1i64 << i);
				}
				else if (i >= 18 && i <= 23)
				{
					features |= (1i64 << (i - 3));
				}
				else
				{
					features |= (1i64 << (i - 6));
				}
			}
		}
		else
		{
			if (result.EDX & (1 << (i - 24)))
			{
				if (i <= 9)
				{
					features |= (1i64 << i);
				}
				else if (i >= 11 && i <= 19)
				{
					features |= (1i64 << (i - 1));
				}
				else
				{
					features |= (1i64 << (i - 2));
				}
			}
		}
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
#define STRINGIFY_CPU_FEATURE(x, predicate, resultVector)	if (predicate(x)) resultVector.push_back(#x)
std::vector<std::string> CPUInfo::featuresAsVector()
{
	std::vector<std::string> featureVector;
	
	STRINGIFY_CPU_FEATURE(SSE3, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(PCLMUL, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(DTES64, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(MONITOR  , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(DS_CPL  , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(VMX  , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(SMX  , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(EST  , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(TM2 , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(SSSE3  , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(CID, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(FMA, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(CX16 , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(ETPRD , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(PDCM , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(DCA, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(SSE4_1 , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(SSE4_2 , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(x2APIC , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(MOVBE , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(POPCNT , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(XSAVE, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(OSXSAVE , supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(AVX, supportsFeature, featureVector);

	STRINGIFY_CPU_FEATURE(FPU, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(VME, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(DE, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(PSE, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(TSC, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(MSR, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(PAE, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(MCE, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(CX8, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(APIC, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(SEP, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(MTRR, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(PGE, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(MCA, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(CMOV, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(PAT, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(PSE36, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(PSN, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(CLF, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(DTES, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(ACPI, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(MMX, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(FXSR, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(SSE, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(SSE2, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(SS, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(HTT, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(TM1, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(IA64, supportsFeature, featureVector);
	STRINGIFY_CPU_FEATURE(PBE, supportsFeature, featureVector);

	return featureVector;
}
#undef STRINGIFY_CPU_FEATURE

std::string CPUInfo::featuresAsString(const char delimiter)
{
	std::string result;
	std::vector<std::string> featureVector = featuresAsVector();
	for (std::vector<std::string>::const_iterator cit = featureVector.begin(); cit != featureVector.end(); ++cit)
	{	
		result += *cit;
		result += delimiter;
	}
	return result;
}

unsigned getNumLogicalProcessorsPerPackage() 
{
	CPUIDinfo result;
	get_cpuid_info(&result, 1);
	return (unsigned) ((result.EBX & 0xFF0000) >> 16);
}

CPUInfo::CPUInfo()
{
	mSupportsCPUID = isCPUIDsupported() == 1 ? true : false;
	if (mSupportsCPUID)
	{
		mCPUString = retrieveCPUString();
		mCPUFeatures = getCPUFeatures();
		mNumLogicalCPUs = getNumLogicalProcessorsPerPackage();
	}
}

CPUInfo::~CPUInfo(){}