/*
*	RendererCommon.hpp
*
*	Common definitions and enums for the renderer subsystem
*
*  Created by Julien Lebot on 17/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __RendererCommon_hpp__
#define __RendererCommon_hpp__

#include <Phoenix/Base.h>

namespace Phoenix
{
	/** Enum identifying the texture type
	*/
	enum TextureTarget
	{
		/// 1D texture, used in combination with 1D texture coordinates
		TEXTURE_1D = 1,
		/// 2D texture, used in combination with 2D texture coordinates (default)
		TEXTURE_2D = 2,
		/// 3D volume texture, used in combination with 3D texture coordinates
		TEXTURE_3D = 3,
		/// 3D cube map, used in combination with 3D texture coordinates
		TEXTURE_CUBE_MAP = 4
	};

	/** High-level filtering options providing shortcuts to settings the
	minification, magnification and mip filters. */
	enum TextureFilterOptions
	{
		/// Equal to: min=FO_POINT, mag=FO_POINT, mip=FO_NONE
		NO_FILTERING,
		/// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_POINT
		BILINEAR_FILTERING,
		/// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_LINEAR
		TRILINEAR_FILTERING,
		/// Equal to: min=FO_ANISOTROPIC, max=FO_ANISOTROPIC, mip=FO_LINEAR
		ANISOTROPIC_FILTERING
	};

	enum FilterType
	{
		/// The filter used when shrinking a texture
		MIN_FILTER,
		/// The filter used when magnifiying a texture
		MAG_FILTER,
		/// The filter used when determining the mipmap
		MIP_FILTER
	};

	/** Filtering options for textures / mipmaps. */
	enum FilterOptions
	{
		/// No filtering, used for FILT_MIP to turn off mipmapping
		FO_NONE,
		/// Use the closest pixel
		FO_POINT,
		/// Average of a 2x2 pixel area, denotes bilinear for MIN and MAG, trilinear for MIP
		FO_LINEAR,
		/// Similar to FO_LINEAR, but compensates for the angle of the texture plane
		FO_ANISOTROPIC
	};

	enum ShadingType
	{
		FLAT_SHADING,
		SMOOTH_SHADING
	};

	enum FogMode
	{
		FOG_DISABLED,
		FOG_EXP,
		FOG_EXP2,
		FOG_LINEAR
	};
}

#endif // __RendererCommon_hpp__