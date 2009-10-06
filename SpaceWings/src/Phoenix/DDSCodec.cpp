/*
*	DDSCodec.cpp
*
*	A DDS loader, code borrowed from http://developer.nvidia.com/object/nature_scene.html
*
*  Modified by Julien Lebot on 11/09/09.
*  Copyright 2005 Nvidia Corp. All rights reserved.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/Image.hpp>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define deleteObject(A){ if(A){ delete   A; A = NULL; } }
#define deleteArray(A) { if(A){ delete[] A; A = NULL; } }

using namespace Phoenix;

#ifndef FREE
#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

#define DDS_FOURCC  0x00000004
#define DDS_RGB     0x00000040
#define DDS_RGBA    0x00000041
#define DDS_VOLUME  0x00200000
#define DDS_CUBEMAP 0x00000200

#define DDS_MAGIC ('D'|('D'<<8)|('S'<<16)|(' '<<24))
#define DDS_DXT1 ('D'|('X'<<8)|('T'<<16)|('1'<<24))
#define DDS_DXT3 ('D'|('X'<<8)|('T'<<16)|('3'<<24))
#define DDS_DXT5 ('D'|('X'<<8)|('T'<<16)|('5'<<24))
/*
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif
*/
typedef struct
{
	unsigned short col0, col1;
	unsigned char row[4];
} DXTColorBlock_t;

typedef struct
{
	unsigned short row[4];
} DXT3AlphaBlock_t;

typedef struct
{
	unsigned char alpha0, alpha1;
	unsigned char row[6];
} DXT5AlphaBlock_t;

typedef struct
{
	unsigned long Size;
	unsigned long Flags;
	unsigned long Height;
	unsigned long Width;
	unsigned long PitchLinearSize;
	unsigned long Depth;
	unsigned long MipMapCount;
	unsigned long Reserved1[11];
	unsigned long pfSize;
	unsigned long pfFlags;
	unsigned long pfFourCC;
	unsigned long pfRGBBitCount;
	unsigned long pfRMask;
	unsigned long pfGMask;
	unsigned long pfBMask;
	unsigned long pfAMask;
	unsigned long Caps1;
	unsigned long Caps2;
	unsigned long Reserved2[3];
} DDS_Header_t;

void Swap(void *byte1, void *byte2, int size)
{
	unsigned char *tmp=(unsigned char *)malloc(sizeof(unsigned char)*size);

	memcpy(tmp, byte1, size);
	memcpy(byte1, byte2, size);
	memcpy(byte2, tmp, size);

	FREE(tmp);
}

void flipDXT1Blocks(DXTColorBlock_t *Block, int NumBlocks)
{
	int i;
	DXTColorBlock_t *ColorBlock=Block;

	for(i=0;i<NumBlocks;i++)
	{
		Swap(&ColorBlock->row[0], &ColorBlock->row[3], sizeof(unsigned char));
		Swap(&ColorBlock->row[1], &ColorBlock->row[2], sizeof(unsigned char));
		ColorBlock++;
	}
}

void flipDXT3Blocks(DXTColorBlock_t *Block, int NumBlocks)
{
	int i;
	DXTColorBlock_t *ColorBlock=Block;
	DXT3AlphaBlock_t *AlphaBlock;

	for(i=0;i<NumBlocks;i++)
	{
		AlphaBlock=(DXT3AlphaBlock_t *)ColorBlock;

		Swap(&AlphaBlock->row[0], &AlphaBlock->row[3], sizeof(unsigned short));
		Swap(&AlphaBlock->row[1], &AlphaBlock->row[2], sizeof(unsigned short));
		ColorBlock++;

		Swap(&ColorBlock->row[0], &ColorBlock->row[3], sizeof(unsigned char));
		Swap(&ColorBlock->row[1], &ColorBlock->row[2], sizeof(unsigned char));
		ColorBlock++;
	}
}

void flipDXT5Alpha(DXT5AlphaBlock_t *Block)
{
	unsigned long *Bits, Bits0=0, Bits1=0;

	memcpy(&Bits0, &Block->row[0], sizeof(unsigned char)*3);
	memcpy(&Bits1, &Block->row[3], sizeof(unsigned char)*3);

	Bits=((unsigned long *)&(Block->row[0]));
	*Bits&=0xff000000;
	*Bits|=(unsigned char)(Bits1>>12)&0x00000007;
	*Bits|=(unsigned char)((Bits1>>15)&0x00000007)<<3;
	*Bits|=(unsigned char)((Bits1>>18)&0x00000007)<<6;
	*Bits|=(unsigned char)((Bits1>>21)&0x00000007)<<9;
	*Bits|=(unsigned char)(Bits1&0x00000007)<<12;
	*Bits|=(unsigned char)((Bits1>>3)&0x00000007)<<15;
	*Bits|=(unsigned char)((Bits1>>6)&0x00000007)<<18;
	*Bits|=(unsigned char)((Bits1>>9)&0x00000007)<<21;

	Bits=((unsigned long *)&(Block->row[3]));
	*Bits&=0xff000000;
	*Bits|=(unsigned char)(Bits0>>12)&0x00000007;
	*Bits|=(unsigned char)((Bits0>>15)&0x00000007)<<3;
	*Bits|=(unsigned char)((Bits0>>18)&0x00000007)<<6;
	*Bits|=(unsigned char)((Bits0>>21)&0x00000007)<<9;
	*Bits|=(unsigned char)(Bits0&0x00000007)<<12;
	*Bits|=(unsigned char)((Bits0>>3)&0x00000007)<<15;
	*Bits|=(unsigned char)((Bits0>>6)&0x00000007)<<18;
	*Bits|=(unsigned char)((Bits0>>9)&0x00000007)<<21;
}

void flipDXT5Blocks(DXTColorBlock_t *Block, int NumBlocks)
{
	DXTColorBlock_t *ColorBlock=Block;
	DXT5AlphaBlock_t *AlphaBlock;
	int i;

	for(i=0;i<NumBlocks;i++)
	{
		AlphaBlock=(DXT5AlphaBlock_t *)ColorBlock;

		flipDXT5Alpha(AlphaBlock);
		ColorBlock++;

		Swap(&ColorBlock->row[0], &ColorBlock->row[3], sizeof(unsigned char));
		Swap(&ColorBlock->row[1], &ColorBlock->row[2], sizeof(unsigned char));
		ColorBlock++;
	}
}

void flip(unsigned char *image, int mWidth, int mHeight, int size, int mFormat)
{
	int linesize, i, j;

	if((mFormat==32)||(mFormat==24))
	{
		unsigned char *top, *bottom;

		linesize=size/mHeight;

		top=image;
		bottom=top+(size-linesize);

		for(i=0;i<(mHeight>>1);i++)
		{
			Swap(bottom, top, linesize);

			top+=linesize;
			bottom-=linesize;
		}
	}
	else
	{
		DXTColorBlock_t *top;
		DXTColorBlock_t *bottom;
		int xblocks=mWidth/4;
		int yblocks=mHeight/4;

		switch(mFormat)
		{
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: 
			linesize=xblocks*8;

			for(j=0;j<(yblocks>>1);j++)
			{
				top=(DXTColorBlock_t *)(image+j*linesize);
				bottom=(DXTColorBlock_t *)(image+(((yblocks-j)-1)*linesize));

				flipDXT1Blocks(top, xblocks);
				flipDXT1Blocks(bottom, xblocks);
				Swap(bottom, top, linesize);
			}
			break;

		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
			linesize=xblocks*16;

			for(j=0;j<(yblocks>>1);j++)
			{
				top=(DXTColorBlock_t *)(image+j*linesize);
				bottom=(DXTColorBlock_t *)(image+(((yblocks-j)-1)*linesize));

				flipDXT3Blocks(top, xblocks);
				flipDXT3Blocks(bottom, xblocks);
				Swap(bottom, top, linesize);
			}
			break;

		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			linesize=xblocks*16;

			for(j=0;j<(yblocks>>1);j++)
			{
				top=(DXTColorBlock_t *)(image+j*linesize);
				bottom=(DXTColorBlock_t *)(image+(((yblocks-j)-1)*linesize));

				flipDXT5Blocks(top, xblocks);
				flipDXT5Blocks(bottom, xblocks);
				Swap(bottom, top, linesize);
			}
			break;

		default:
			return;
		}
	}
}

bool Image::loadDDS()
{
	DDS_Header_t dds;
	unsigned long magic,
		bits = 0;
	FILE *stream;
	int size;

	if((stream=fopen(mPath.file_string().c_str(), "rb"))==NULL)
		return 0;

	fread(&magic, sizeof(unsigned long), 1, stream);

	if(magic!=DDS_MAGIC)
	{
		fclose(stream);
		return 0;
	}

	fread(&dds, sizeof(DDS_Header_t), 1, stream);

	if(dds.pfFlags&DDS_FOURCC)
	{
		switch(dds.pfFourCC)
		{
		case DDS_DXT1:
			mInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			mComponents     = 4;
			bits           = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;

		case DDS_DXT3:
			mInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			mComponents     = 4;
			bits           = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;

		case DDS_DXT5:
			mInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			mComponents     = 4;
			bits           = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;

		default:
			fclose(stream);
			return 0;
		}
	}
	else if (dds.pfFlags == DDS_RGBA && dds.pfRGBBitCount == 32)
	{
		bits           = 32;
		mFormat         = GL_BGRA;
		mComponents     =  4;
		mInternalFormat = GL_RGBA;
	}
	else if (dds.pfFlags == DDS_RGB  && dds.pfRGBBitCount == 32)
	{
		bits           = 32;
		mFormat         = GL_BGRA;
		mComponents     =  4;
		mInternalFormat = GL_RGBA;
	}
	else if (dds.pfFlags == DDS_RGB  && dds.pfRGBBitCount == 24)
	{
		bits           =     24;
		mFormat         = GL_BGR;
		mComponents     =      3;
		mInternalFormat = GL_RGB;
	}
	else 
	{
		fclose(stream);
		return false;
	}

	mWidth  = unsigned short(dds.Width);
	mHeight = unsigned short(dds.Height);

	if ((dds.Caps2 & DDS_VOLUME) && (dds.Depth > 0))
		mDepth  = unsigned short(dds.Depth);

	if((bits ==32 )||(bits == 24)){
		size = mWidth*mHeight*(bits/8);
	}
	else
	{
		if(bits == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
			size=((mWidth+3)/4)*((mHeight+3)/4)*8;
		else
			size=((mWidth+3)/4)*((mHeight+3)/4)*16;
	}

	if(mBuffer)
		deleteArray(mBuffer);

	int sizeOfBuffer = size *(!mDepth ? 1 : mDepth);
	mBuffer       = new GLubyte[sizeOfBuffer];

	if(!mBuffer)
	{
		fclose(stream);
		return 0;
	}

	if(mDepth)
	{
		for(int c = 0; c < mDepth; c++)
			fread(mBuffer + c*size, sizeof(unsigned char), size, stream);
	}
	else
	{
		fread(mBuffer, sizeof(unsigned char), size, stream);
		flip(mBuffer, mWidth, mHeight, size, bits);
	}

	fclose(stream);
	return true;
}
