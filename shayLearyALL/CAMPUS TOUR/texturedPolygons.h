//  texturedPolygon.h
//  Header file for the texturedPolygon class
// 
//
//  Shay Leary, March 2005
//--------------------------------------------------------------------------------------

#ifndef TEXTURE_POLYGON_H
#define TEXTURE_POLYGON_H

//--------------------------------------------------------------------------------------

#include <gl/glut.h>
#include <stdio.h>	// should really be using c++, but didn't get time to change
#include <stdlib.h>

//--------------------------------------------------------------------------------------

class TexturedPolygons
{
public:

	TexturedPolygons () {}
    virtual ~TexturedPolygons () {Clear();}

	//----------------------------------------------------------------------------------

	// returns texture
	GLuint GetTexture (const int & tempIndex) {return m_texture[tempIndex];}

	// loads a raw image file into memory
	GLubyte* LoadTexture(char *filename, int imgWidth, int imgHeight);
	
	// set the number of textures to be used 
	void SetTextureCount(const int &textureNo);

	// creates texture and set required values for texture mapping
	void CreateTexture(int textureNo, unsigned char* image, int imgWidth, int imgHeight);

	// calls function to creates display list to contain image
	void CreateDisplayList(const int &XYZ, const int &listNo, 
		                   const GLdouble &xImgSize, const GLdouble &zImgSize, 
					       const GLdouble &xStart, const GLdouble &yStart, const GLdouble &zStart,
					       const GLdouble &xTimes, const GLdouble &zTimes);

	// used to create display lists where image is larger than world scale on YZ axis
	void CreateYtoZWindowList(const int &listNo, const GLdouble &xStart, 
							  const GLdouble &yStart, const GLdouble &ySize, 
							  const GLdouble &zStart, const GLdouble &zSize,
							  const GLdouble &yImgSize, const GLdouble &zImgSize);

	// used to create display lists where image is larger than world scale on XY axis
	void CreateXtoYWindowList(const int &listNo, const GLdouble &zStart, 
							  const GLdouble &xStart, const GLdouble &xSize, 
							  const GLdouble &yStart, const GLdouble &ySize,
							  const GLdouble &xImgSize, const GLdouble &yImgSize);
	
	// used to create display lists where image is on an angle
	void CreateAngledPolygon(const int &listNo, 
							 const GLdouble &imageWidth, const GLdouble &imageHeight,
							 const GLdouble &x1, const GLdouble &x2, 
							 const GLdouble &x3, const GLdouble &x4,
						     const GLdouble &y1, const GLdouble &y2,
							 const GLdouble &y3, const GLdouble &y4,
							 const GLdouble &z1, const GLdouble &z2,
							 const GLdouble &z3, const GLdouble &z4,
							 const int &smallestX, const int &smallestZ);

private:
	// pointer to display list
	GLuint *m_texture;

	// returns image file
	GLubyte* LoadRawImageFile(char* filename, int width, int height);

	void Clear ();

	// creates display list to contain image on X Z axis
	void CreateXtoZTextureList(const GLdouble &xImgSize, const GLdouble &zImgSize, 
					           const GLdouble &xStart, const GLdouble &yStart, const GLdouble &zStart,
					           const GLdouble &xTimes, const GLdouble &zTimes);

	// creates display list to contain image on X Y axis
	void CreateXtoYTextureList(const GLdouble &xImgSize, const GLdouble &yImgSize, 
							   const GLdouble &xStart, const GLdouble &yStart, const GLdouble &zStart,
							   const GLdouble &xTimes, const GLdouble &yTimes, const bool &flip);

	// creates display list to contain image on Y Z axis
	void CreateYtoZTextureList(const GLdouble &yImgSize, const GLdouble &zImgSize, 
							   const GLdouble &xStart, const GLdouble &yStart, const GLdouble &zStart,
							   const GLdouble &yTimes, const GLdouble &zTimes, const bool &flip);
	
	// called from	CreateAngledPolygon determine how images are displayed
	void CreateTextureScale(GLdouble & xzImage1, GLdouble & xzImage2, 
							GLdouble & xzImage3, GLdouble & xzImage4,
							const GLdouble &imageSize);

	//----------------------------------------------------------------------------------

    // Privatised copy constructor and assignment operator
    TexturedPolygons (const TexturedPolygons &tp) {};
    TexturedPolygons &operator = (const TexturedPolygons &tp) {};
};

#endif

//----------------------------------------------------------------------------------