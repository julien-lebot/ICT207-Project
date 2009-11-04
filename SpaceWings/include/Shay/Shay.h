#ifndef __Shay_h__
#define __Shay_h__

#include <Phoenix/Base.h>
#include <Shay/Camera.h>

namespace Shay
{
	class ShayWorld
	{
	protected:
		// USE THESE STTEINGS TO CHANGE SPEED (on different spec computers)
		// Set speed (steps)
		GLdouble movementSpeed;
		GLdouble rotationSpeed;

		GLdouble stepIncrement;
		GLdouble angleIncrement;
		int frameCount;

		// ratio of screen
		float ratio;
		// screen width and height
		int width, height;

		// display campus map
		bool DisplayMap;
		// display welcome screen
		bool DisplayWelcome;
		// display exit screen
		bool DisplayExit;
		// display light fittings
		bool lightsOn;
		// display ECL block
		bool displayECL;

		// variables used for translating graphics etc //spelling
		GLdouble step, step2, stepLength;

		// Glut Cylinder
		GLUquadricObj *glu_cylinder;

		// objects
		clock_t lastClock;
		
		//Shay camera
		Camera cam;

	public:
		// Stores raw image file
		typedef Phoenix::tr1::shared_ptr<unsigned char> ImagePtr;
//#define ImagePtr

		ShayWorld();

		// initializes setting
		void myinit();
		
		// calls display functions below to draw the backdrops
		void DrawBackdrop();
		// functions to display display lists (images) and bind them to a texture
		void DisplayAboveWindowBlock ();
		void DisplayBench ();
		void DisplayBricks ();
		void DisplayChancPosts ();
		void DisplayCylinders ();
		void DisplayDoorPaving ();
		void DisplayDoorPosts ();
		void DisplayEntranceSteps ();
		void DisplayExtras ();
		void DisplayGrass ();
		void DisplayLargerTextures ();
		void DisplayLibraryPosts ();
		void DisplayMainPosts ();
		void DisplayPavement ();
		void DisplayPhysSciPosts ();
		void DisplayPurplePosts ();
		void DisplayRedPosts ();
		void DisplayRoof();
		void DisplayStepBricks ();
		void DisplayLights ();
		void DisplayECL ();

		// calls functions to create display lists (below)
		void CreateTextureList();
		// creates display lists
		void DrawGrass ();
		void DrawChancPosts ();
		void DrawDoorPosts ();
		void DrawPurplePosts ();
		void DrawRedPosts ();
		void DrawMainPosts ();
		void DrawAboveWindowBlock ();
		void DrawDoorPaving ();
		void DrawPhysSciPosts ();
		void DrawLibraryPosts ();
		void DrawBricks ();
		void DrawPavement ();
		void DrawExtras ();
		void DrawRoof();
		void DrawEntranceSteps ();
		void DrawLargerTextures ();
		void DrawLights ();
		void DrawBench ();
		void DrawCylinders ();
		void DrawAngledRoofBeam (int listNo, GLfloat x, GLfloat y, GLfloat z, GLfloat beamSize);
		void DrawAngledRoofBeam2 (int listNo, GLfloat x, GLfloat y, GLfloat z, GLfloat beamSize);
		void DrawStepBricks ();
		void DrawMapExit ();
		void DrawECL ();


		void BindBridgeWall(GLint LR);
		void BindBuildingWall();
		void BindWallPosts(GLint LR);

		void IncrementFrameCount();

		// loads images and creates texture
		void CreateTextures();
		// creates bounding boxes for collsion detection
		void CreateBoundingBoxes();
		// creates different plains
		void CreatePlains();

		// deletes image and clears memory
		void DeleteImageFromMemory(unsigned char* tempImage);
	};
}

#endif // __Shay_h__