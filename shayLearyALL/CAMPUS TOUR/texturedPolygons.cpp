//  texturedPolygons.cpp
//
//  Implementation file for TexturedPolygons Class
//  Defines all the methods declared, but not defined, in texturedPolygons.h
//
//  Shay Leary, March 2005
//--------------------------------------------------------------------------------------

#include <iostream>
#include "texturedPolygons.h"


//--------------------------------------------------------------------------------------
//  Declares datatype to store a raw image file and calls method to load image
//--------------------------------------------------------------------------------------

GLubyte* TexturedPolygons::LoadTexture(char* filename, int imgWidth, int imgHeight)
{
	//
	unsigned char* image = NULL;
	image = LoadRawImageFile(filename, imgWidth, imgHeight);
	// inform user if file loaded
	std::cout << "Loading image file " << filename << "...\n";
	return image;
}

//--------------------------------------------------------------------------------------
//  Creates memory space to store raw image file and reads in file from disk.
//--------------------------------------------------------------------------------------

GLubyte* TexturedPolygons::LoadRawImageFile(char* filename, int width, int height)
{
	FILE* file;
	unsigned char* image;
	// create memory space w x h x 3 (3 stores RGB values)
	image = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 3);
	file = fopen(filename, "rb" );
	// exit program if image not found and inform user
	if (file == NULL)
	{
		std::cout << "ERROR loading image file: " << filename << "...\n";
		exit(0);
	}
	fread(image, width * height * 3, 1, file);
	fclose(file);
    return image;
}

//--------------------------------------------------------------------------------------
//  Set number of textures to be used in program
//--------------------------------------------------------------------------------------

void TexturedPolygons::SetTextureCount(const int &textureNo)
{
	m_texture = new GLuint[textureNo];
	glGenTextures(textureNo, &m_texture[0]);
}

//--------------------------------------------------------------------------------------
//  Clears memory used to store textures when program terminates (very important)
//--------------------------------------------------------------------------------------

void TexturedPolygons::Clear()
{
	m_texture = NULL;
	if (m_texture == NULL)
	{
		delete [] m_texture;
	}
}

//--------------------------------------------------------------------------------------
//  Creates texture and set required values for texture mapping
//--------------------------------------------------------------------------------------

void TexturedPolygons::CreateTexture(int textureNo, unsigned char* image, int imgWidth, int imgHeight)
{
	glBindTexture(GL_TEXTURE_2D, m_texture[textureNo]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, image);
}

//--------------------------------------------------------------------------------------
//  Calls functions to create display lists, depending on parameters.
//  I created these functions very early on in the program before I had a full understanding,
//  therefore I would probably create a better function for the purpose, but this works well.
//--------------------------------------------------------------------------------------

void TexturedPolygons::CreateDisplayList(const int &XYZ, const int &listNo, 
		                                 const GLdouble &xImgSize, const GLdouble &zImgSize, 
					                     const GLdouble &xStart, const GLdouble &yStart, const GLdouble &zStart,
					                     const GLdouble &xTimes, const GLdouble &zTimes)
{
	glNewList(listNo, GL_COMPILE);
		glBegin(GL_QUADS);
		switch (XYZ)
		{
			case 0:
				// create list where image is on X and Y axis
				CreateXtoYTextureList(xImgSize, zImgSize, xStart, yStart, zStart, xTimes, zTimes, false);
				break;
			case 1:
				// create list where image is on X and Z axis
				CreateXtoZTextureList(xImgSize, zImgSize, xStart, yStart, zStart, xTimes, zTimes);
				break;
			case 2:
				// create list where image is on Y and Z axis
				CreateYtoZTextureList(xImgSize, zImgSize, xStart, yStart, zStart, xTimes, zTimes, false);
				break;
			case 3:
				// create list where (flipped) image is on X and Z axis
				CreateYtoZTextureList(xImgSize, zImgSize, xStart, yStart, zStart, xTimes, zTimes, true);
				break;
			case 4:
				// create list where (flipped) image is on X and Y axis
				CreateXtoYTextureList(xImgSize, zImgSize, xStart, yStart, zStart, xTimes, zTimes, true);
				break;
		}
		glEnd();
	glEndList();
}

//--------------------------------------------------------------------------------------
//  Create display list with image plotted on X to Z axis
//--------------------------------------------------------------------------------------

void TexturedPolygons::CreateXtoZTextureList(const GLdouble &xImgSize, const GLdouble &zImgSize, 
					                         const GLdouble &xStart, const GLdouble &yStart, const GLdouble &zStart,
					                         const GLdouble &xTimes, const GLdouble &zTimes)
{
	glTexCoord2f(0.0, 0.0);
	glVertex3f(xStart, yStart , zStart);
	glTexCoord2f(0.0, zTimes);
	glVertex3f(xStart, yStart,zStart + (zImgSize * zTimes));
	glTexCoord2f(xTimes, zTimes);
	glVertex3f(xStart + (xImgSize * xTimes), yStart,zStart + (zImgSize * zTimes));
	glTexCoord2f(xTimes, 0.0);
	glVertex3f(xStart + (xImgSize * xTimes), yStart, zStart);
}

//--------------------------------------------------------------------------------------
//  Create display list with image plotted on X to Y axis
//--------------------------------------------------------------------------------------

void TexturedPolygons::CreateXtoYTextureList(const GLdouble &xImgSize, const GLdouble &yImgSize, 
							                 const GLdouble &xStart, const GLdouble &yStart, const GLdouble &zStart,
							                 const GLdouble &xTimes, const GLdouble &yTimes, const bool &flip)
{
	GLdouble flipX = 0.0;
	GLdouble tempX = xTimes;
	// if image is required to be flipped (this will be improved)
	if (flip)
	{
		flipX = xTimes;
		tempX = 0.0;
	}
	glTexCoord2f(flipX, 0.0);
	glVertex3f(xStart, yStart, zStart);
	glTexCoord2f(flipX, yTimes);   
	glVertex3f(xStart, yStart + (yImgSize * yTimes), zStart);
	glTexCoord2f(tempX, yTimes);
	glVertex3f(xStart + (xImgSize * xTimes), yStart + (yImgSize * yTimes), zStart);
	glTexCoord2f(tempX, 0.0);
	glVertex3f(xStart + (xImgSize * xTimes), yStart, zStart);
}

//--------------------------------------------------------------------------------------
//  Create display list with image plotted on Y to A axis
//--------------------------------------------------------------------------------------

void TexturedPolygons::CreateYtoZTextureList(const GLdouble &yImgSize, const GLdouble &zImgSize, 
							                 const GLdouble &xStart, const GLdouble &yStart, const GLdouble &zStart,
							                 const GLdouble &yTimes, const GLdouble &zTimes, const bool &flip)
{
	GLdouble flipZ = 0.0;
	GLdouble tempZ = zTimes;
	// if image is required to be flipped
	if (flip)
	{
		flipZ = zTimes;
		tempZ = 0.0;
	}
	glTexCoord2f(0.0, flipZ);
	glVertex3f(xStart, yStart, zStart);
	glTexCoord2f(0.0, tempZ);
	glVertex3f(xStart, yStart, zStart + (zImgSize * zTimes));
	glTexCoord2f(yTimes, tempZ);
	glVertex3f(xStart,yStart + (yImgSize * yTimes), zStart + (zImgSize * zTimes));
	glTexCoord2f(yTimes, flipZ);
	glVertex3f(xStart, yStart + (yImgSize * yTimes), zStart);	
}

//--------------------------------------------------------------------------------------
//  Used to create display lists where the image size is not to scale with the world
//  co-ordinates.
//  This is mainly used to create lists for the larger images (i.e. windows and doors),
//  where they are to large to keep to scale.
//--------------------------------------------------------------------------------------

void TexturedPolygons::CreateYtoZWindowList(const int &listNo, const GLdouble &xStart, 
											const GLdouble &yStart, const GLdouble &ySize, 
											const GLdouble &zStart, const GLdouble &zSize,
											const GLdouble &yImgSize, const GLdouble &zImgSize)
{
	glNewList(listNo, GL_COMPILE);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, zImgSize);
			glVertex3f(xStart, yStart, zStart);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(xStart, yStart, zStart + zSize);
			glTexCoord2f(yImgSize, 0.0);
			glVertex3f(xStart,yStart + ySize, zStart + zSize);
			glTexCoord2f(yImgSize, zImgSize);
			glVertex3f(xStart, yStart + ySize, zStart);	
		glEnd();
	glEndList();
}

//--------------------------------------------------------------------------------------

void TexturedPolygons::CreateXtoYWindowList(const int &listNo, const GLdouble &zStart, 
											const GLdouble &xStart, const GLdouble &xSize, 
											const GLdouble &yStart, const GLdouble &ySize,
											const GLdouble &xImgSize, const GLdouble &yImgSize)
{
	glNewList(listNo, GL_COMPILE);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(xStart, yStart, zStart);
			glTexCoord2f(xImgSize, 0.0);
			glVertex3f(xStart + xSize, yStart, zStart);
			glTexCoord2f(xImgSize, yImgSize);
			glVertex3f(xStart + xSize, yStart + ySize, zStart);
			glTexCoord2f(0.0, yImgSize);
			glVertex3f(xStart, yStart + ySize, zStart);	
		glEnd();
	glEndList();
}

//--------------------------------------------------------------------------------------
//  Used to create display lists where the image is on an angle.
//  The final two parameters are used to rotate the image so that it appears correctly.
//  This works by passing the number of the smallest co-ordinate.
//  Eg, smallestX = 1 to 4 for smallest x values (image angles on the XZ or XY axis)
//      smallestX = 5 to 8 for smallest y values (image angles on the YZ)
//      smallestZ = 1 to 4 for smallest z values (image angles on the XZ or YZ)
//      smallestZ = 5 to 8 for smallest y values (image angles on the XY)
//--------------------------------------------------------------------------------------

void TexturedPolygons::CreateAngledPolygon(const int &listNo, 
										   const GLdouble &imageWidth, const GLdouble &imageHeight,
										   const GLdouble &x1, const GLdouble &x2, 
										   const GLdouble &x3, const GLdouble &x4,
										   const GLdouble &y1, const GLdouble &y2,
										   const GLdouble &y3, const GLdouble &y4,
										   const GLdouble &z1, const GLdouble &z2,
										   const GLdouble &z3, const GLdouble &z4,
										   const int &smallestX, const int &smallestZ)
{
	GLdouble xImage1 = x1;
	GLdouble xImage2 = x2;
	GLdouble xImage3 = x3;
	GLdouble xImage4 = x4;
	GLdouble zImage1 = z1;
	GLdouble zImage2 = z2;
	GLdouble zImage3 = z3;
	GLdouble zImage4 = z4;

	if (smallestX == 1)
	{
		CreateTextureScale(xImage1, xImage2, xImage3, xImage4, imageWidth);
	}
	else if (smallestX == 2)
	{
		CreateTextureScale(xImage2, xImage1, xImage3, xImage4, imageWidth);
	}
	else if (smallestX == 3)
	{
		CreateTextureScale(xImage3, xImage1, xImage2, xImage4, imageWidth);
	}
	else if (smallestX == 4)
	{
		CreateTextureScale(xImage4, xImage1, xImage2, xImage3, imageWidth);
	}
	else
	{
		xImage1 = y1;
		xImage2 = y2;
		xImage3 = y3;
		xImage4 = y4;
		if (smallestX == 5)
		{
			CreateTextureScale(xImage1, xImage2, xImage3, xImage4, imageWidth);
		}
		else if (smallestX == 6)
		{
			CreateTextureScale(xImage2, xImage1, xImage3, xImage4, imageWidth);
		}
		else if (smallestX == 7)
		{
			CreateTextureScale(xImage3, xImage1, xImage2, xImage4, imageWidth);
		}
		else if (smallestX == 8)
		{
			CreateTextureScale(xImage4, xImage1, xImage2, xImage3, imageWidth);
		}
	}	
	if (smallestZ == 1)
	{
		CreateTextureScale(zImage1, zImage2, zImage3, zImage4, imageHeight);
	}
	else if (smallestZ == 2)
	{
		CreateTextureScale(zImage2, zImage1, zImage3, zImage4, imageHeight);
	}
	else if (smallestZ == 3)
	{
		CreateTextureScale(zImage3, zImage1, zImage2, zImage4, imageHeight);
	}
	else if (smallestZ == 4)
	{
		CreateTextureScale(zImage4, zImage1, zImage2, zImage3, imageHeight);
	}
	else
	{
		zImage1 = y1;
		zImage2 = y2;
		zImage3 = y3;
		zImage4 = y4;
		if (smallestZ == 5)
		{
			CreateTextureScale(zImage1, zImage2, zImage3, zImage4, imageHeight);
		}
		else if (smallestZ == 6)
		{
			CreateTextureScale(zImage2, zImage1, zImage3, zImage4, imageHeight);
		}
		else if (smallestZ == 7)
		{
			CreateTextureScale(zImage3, zImage1, zImage2, zImage4, imageHeight);
		}
		else if (smallestZ == 8)
		{
			CreateTextureScale(zImage4, zImage1, zImage2, zImage3, imageHeight);
		}
	}
	// create display list
	glNewList(listNo, GL_COMPILE);
		glBegin(GL_QUADS);
			glTexCoord2f(xImage1, zImage1);
			glVertex3f(x1, y1, z1);
			glTexCoord2f(xImage2, zImage2);
			glVertex3f(x2, y2, z2);
			glTexCoord2f(xImage3, zImage3);
			glVertex3f(x3, y3, z3);
			glTexCoord2f(xImage4, zImage4);
			glVertex3f(x4, y4, z4);
		glEnd();
	glEndList();

}

//--------------------------------------------------------------------------------------
//  Called from	CreateAngledPolygon determine how images are displayed
//--------------------------------------------------------------------------------------

void TexturedPolygons::CreateTextureScale(GLdouble & xzImage1, GLdouble & xzImage2, 
										  GLdouble & xzImage3, GLdouble & xzImage4,
										  const GLdouble &imageSize)
{
	xzImage2 = ((xzImage2 - xzImage1) / imageSize);
	xzImage3 = ((xzImage3 - xzImage1) / imageSize);
	xzImage4 = ((xzImage4 - xzImage1) / imageSize);
	xzImage1 = 0.0;
}

//--------------------------------------------------------------------------------------