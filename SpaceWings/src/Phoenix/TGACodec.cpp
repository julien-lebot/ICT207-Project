/*
*	TGACodec.cpp
*
*	A TGA loader, code borrowed from http://developer.nvidia.com/object/nature_scene.html
*
*  Modified by Julien Lebot on 11/09/09.
*  Copyright 2005 Nvidia Corp. All rights reserved.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/Image.hpp>

#define deleteObject(A){ if(A){ delete   A; A = NULL; } }
#define deleteArray(A) { if(A){ delete[] A; A = NULL; } }

using namespace Phoenix;

bool Image::loadTGA()
{
	unsigned char uncompressed8BitTGAHeader[12]= {0, 1, 1, 0, 0, 0, 1, 24, 0, 0, 0, 0},
		uncompressedTGAHeader[12]    = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		compressedTGAHeader[12]      = {0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	unsigned char TGAcompare[12];           //Used to compare TGA header

	FILE * file = fopen(mPath.file_string().c_str(), "rb");    //Open the TGA file

	if(file==NULL )                         //Does the file exist?
		return false;	// TODO: log

	//read the header
	fread(TGAcompare, 1, sizeof(TGAcompare), file);
	fclose(file);

	if(!memcmp(uncompressedTGAHeader, TGAcompare, sizeof(uncompressedTGAHeader)))
	{
		return loadUncompressedTrueColorTGA();
	}
	else if(!memcmp(compressedTGAHeader, TGAcompare, sizeof(compressedTGAHeader)))
	{
		return loadCompressedTrueColorTGA();
	}
	else if(!memcmp(uncompressed8BitTGAHeader, TGAcompare, sizeof(uncompressed8BitTGAHeader)))
	{
		return loadUncompressed8BitTGA();
	}
	else
		return false; // TODO: log

	return false;
}

bool Image::loadUncompressed8BitTGA()
{
	unsigned char   TGAHeader[12]={0, 1, 1, 0, 0, 0, 1, 24, 0, 0, 0, 0};
	unsigned char   TGAcompare[12];           //Used to compare TGA header
	unsigned char   header[6];              //First 6 useful bytes of the header

	FILE * file = fopen(mPath.file_string().c_str(), "rb");        //Open the TGA file

	if(file == NULL)                //Does the file exist?
		return false; // TODO: log

	if(fread(TGAcompare, 1, sizeof(TGAcompare), file)!=sizeof(TGAcompare)|| //Are there 12 bytes to read?
		memcmp(TGAHeader, TGAcompare, sizeof(TGAHeader))!=0 ||          //Is the header correct?
		fread(header, 1, sizeof(header), file)!=sizeof(header))   //Read next 6 bytes
	{
		fclose(file);               //If anything else failed, close the file
		return false; // TODO: log
	}

	//save data into class member variables
	setWidth(header[1]*256+header[0]);
	setHeight(header[3]*256+header[2]);
	setComponentsCount(header[4]/8);

	if( mWidth  <=0 || //if mWidth <=0
		mHeight <=0 || //or mHeight<=0
		header[4] != 8)    //bpp not 8
	{
		fclose(file);                     //close the file
		return false; // TODO: log
	}

	setFormat(GL_RGB);
	setInternalFormat(GL_RGB8);

	//make space for palette
	unsigned char * palette=new unsigned char[256*3];
	if(!palette)
		return false; // TODO: log


	//load the palette
	fread(palette, 256*3, 1, file);

	//allocate space for color indices
	unsigned char * indices=new unsigned char[mWidth*mHeight];
	if(!indices)
		return false; // TODO: log

	//load indices
	fread(indices, 1, mWidth*mHeight, file);

	//close the file
	fclose(file);

	if(mBuffer)
		deleteArray(mBuffer);

	//allocate space for the image data
	mBuffer = new GLubyte[mWidth*mHeight*3];

	if(!mBuffer)
	{
		fclose(file);
		return false; // TODO: log
	}

	//calculate the color values
	for(int currentRow=0; currentRow<mHeight; currentRow++)
	{
		for(int i=0; i<mWidth; i++)
		{
			mBuffer[(currentRow*mWidth+i)*3+0]=palette[indices[currentRow*mWidth+i]*3+2];
			mBuffer[(currentRow*mWidth+i)*3+1]=palette[indices[currentRow*mWidth+i]*3+1];
			mBuffer[(currentRow*mWidth+i)*3+2]=palette[indices[currentRow*mWidth+i]*3+0];//BGR
		}
	}
	return true;
}

bool Image::loadUncompressedTrueColorTGA()
{
	unsigned char TGAheader[12]={0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Uncompressed TGA header
	unsigned char TGAcompare[12];           //Used to compare TGA header
	unsigned char header[6];              //First 6 useful bytes of the header
	unsigned int  imageSize;              //Stores Image size when in RAM

	FILE * file = fopen(mPath.file_string().c_str(), "rb");        //Open the TGA file

	if(file == NULL)                //Does the file exist?
		return false; // TODO: log

	if( fread(TGAcompare, 1, sizeof(TGAcompare), file)!=sizeof(TGAcompare)||  //Are there 12 bytes to read?
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader))!=0 ||          //Is the header correct?
		fread(header, 1, sizeof(header), file)!=sizeof(header))   //Read next 6 bytes
	{
		fclose(file);               //If anything else failed, close the file
		return false; // TODO: log
	}

	//save data into class member variables
	setWidth(header[1]*256+header[0]);           //determine the image mWidth
	setHeight(header[3]*256+header[2]);            //determine image mHeight
	setComponentsCount(header[4]/3);

	if(mWidth <=0 ||                      //if mWidth <=0
		mHeight<=0 ||                      //or mHeight<=0
		(header[4] !=24 && header[4]!=32))                   //bpp not 24 or 32
	{
		fclose(file);                     //close the file
		return false; // TODO: log
	}

	//set format
	if(header[4] == 24){
		setFormat(GL_BGR);
		setInternalFormat(GL_RGB8);
	}
	else{
		setFormat(GL_BGRA);
		setInternalFormat(GL_RGBA8);
	}

	imageSize = mWidth*mHeight*getComponentsCount();

	if(mBuffer)
		deleteArray(mBuffer);

	mBuffer = new GLubyte[imageSize];

	if(mBuffer==NULL)                     //Does the storage memory exist?
	{
		fclose(file);
		return false; // TODO: log
	}

	//read in the image data
	fread(mBuffer, 1, imageSize, file);
	fclose(file);
	return true;
}

//load a compressed TGA texture (24 or 32 bpp)
bool Image::loadCompressedTrueColorTGA()
{
	unsigned char TGAheader[12]={0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //Compressed TGA header
	unsigned char TGAcompare[12];           //Used to compare TGA header
	unsigned char header[6];              //First 6 useful bytes of the header
	unsigned int  bytesPerPixel;            //bytes per pixel
	unsigned int  imageSize;              //Stores Image size when in RAM

	FILE * file = fopen(mPath.file_string().c_str(), "rb");        //Open the TGA file

	if(file == NULL)                //Does the file exist?
		return false; // TODO: log


	if( fread(TGAcompare, 1, sizeof(TGAcompare), file)!=sizeof(TGAcompare)||  //Are there 12 bytes to read?
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader))!=0 ||          //Is the header correct?
		fread(header, 1, sizeof(header), file)!=sizeof(header))   //Read next 6 bytes
	{
		fclose(file);               //If anything else failed, close the file
		return false; // TODO: log
	}

	//save data into class member variables
	setWidth(header[1]*256+header[0]);            //determine the image mWidth
	setHeight(header[3]*256+header[2]);            //determine image mHeight
	setComponentsCount(header[4]/8);
	bytesPerPixel = getComponentsCount();

	if(mWidth  <=0 ||                      //if mWidth <=0
		mHeight <=0 ||                      //or mHeight<=0
		(header[4] !=24 && header[4] !=32))                   //bpp not 24 or 32
	{
		fclose(file);                     //close the file
		return false; // TODO: log
	}

	//set format
	if(header[4] == 24)
	{
		setFormat(GL_RGB);
		setInternalFormat(GL_RGB8);
	}
	else
	{
		setFormat(GL_RGBA);
		setInternalFormat(GL_RGBA8);
	}

	imageSize = mWidth*mHeight*getComponentsCount();

	if(mBuffer)
		deleteArray(mBuffer);
	mBuffer = new GLubyte[imageSize];

	if(!mBuffer)                         //Does the storage memory exist?
	{
		fclose(file);
		return false; // TODO: log
	}

	//read in the image data
	int pixelCount  = mWidth*mHeight;
	int currentPixel= 0;
	int currentByte = 0;
	GLubyte *colorBuffer = new GLubyte[getComponentsCount()];

	do
	{
		unsigned char chunkHeader=0;

		if(!fread(&chunkHeader, sizeof(unsigned char), 1, file))
		{
			if(file)
				fclose(file);
			deleteArray(mBuffer);
			return false; // TODO: log
		}

		if(chunkHeader<128) //Read raw color values
		{
			chunkHeader++;

			for(short counter=0; counter<chunkHeader; counter++)
			{
				if(fread(colorBuffer, 1, bytesPerPixel, file) != bytesPerPixel)
				{
					if(file)
						fclose(file);
					deleteArray(colorBuffer);
					deleteArray(mBuffer);
					return false; // TODO: log
				}

				//transfer pixel color to data (swapping r and b values)
				mBuffer[currentByte]   = colorBuffer[2];
				mBuffer[currentByte+1] = colorBuffer[1];
				mBuffer[currentByte+2] = colorBuffer[0];

				if(bytesPerPixel==4)
					mBuffer[currentByte+3]=colorBuffer[3];

				currentByte+=bytesPerPixel;
				currentPixel++;

				if(currentPixel > pixelCount)
				{
					if(file)
						fclose(file);
					deleteArray(colorBuffer);
					deleteArray(mBuffer);
					return false; // TODO: log
				}
			}
		}
		else  //chunkHeader>=128
		{
			chunkHeader-=127;

			if(fread(colorBuffer, 1, bytesPerPixel, file) != bytesPerPixel)
			{
				if(file)
					fclose(file);
				deleteArray(colorBuffer);
				deleteArray(mBuffer);
				return false; // TODO: log
			}

			for(short counter=0; counter<chunkHeader; counter++)
			{
				//transfer pixel color to data (swapping r and b values)
				mBuffer[currentByte]   = colorBuffer[2];
				mBuffer[currentByte+1] = colorBuffer[1];
				mBuffer[currentByte+2] = colorBuffer[0];

				if(bytesPerPixel==4)
					mBuffer[currentByte+3]=colorBuffer[3];

				currentByte+=bytesPerPixel;
				currentPixel++;

				if(currentPixel > pixelCount)
				{
					if(file)
						fclose(file);
					deleteArray(colorBuffer);
					deleteArray(mBuffer);
					return false; // TODO: log
				}
			}
		}
	} while(currentPixel<pixelCount);

	fclose(file);
	deleteArray(colorBuffer);
	return true;
}
