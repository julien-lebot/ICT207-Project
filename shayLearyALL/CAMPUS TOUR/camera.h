//  camera.h
//  Header file for the camera class
//	Allows the camera to be navigated around the world. 
//	Also sets Plains, Bounding Boxes and view Maps
//
//  Shay Leary, March 2005
//--------------------------------------------------------------------------------------
#ifndef CAMERA_H
#define CAMERA_H

#define PI 3.1415962654

#include "collision.h"
#include "cameraMap.h"
#include "PlainLinkedList.h"
#include "EasySound.h"

//--------------------------------------------------------------------------------------

class Camera
{
public:

	Camera();
	virtual ~Camera() {es->Unload(stepSound);}

	//----------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------
	//  Set Methods
	//----------------------------------------------------------------------------------
	// sets initial value for bounding boxes (in the array AABB)
	void SetAABBMaxX(const int & tempIndex, const GLdouble &tempX) {m_colDetect.SetAABBMaxX(tempIndex, tempX);}
	void SetAABBMinX(const int & tempIndex, const GLdouble &tempX) {m_colDetect.SetAABBMinX(tempIndex, tempX);}
	void SetAABBMaxY(const int & tempIndex, const GLdouble &tempY) {m_colDetect.SetAABBMaxY(tempIndex, tempY);}
	void SetAABBMinY(const int & tempIndex, const GLdouble &tempY) {m_colDetect.SetAABBMinY(tempIndex, tempY);}
	void SetAABBMaxZ(const int & tempIndex, const GLdouble &tempZ) {m_colDetect.SetAABBMaxZ(tempIndex, tempZ);}
	void SetAABBMinZ(const int & tempIndex, const GLdouble &tempZ) {m_colDetect.SetAABBMinZ(tempIndex, tempZ);}

	// set step and rotation size
	void SetRotateSpeed (const GLdouble &tempSpeed) {m_rotateSpeed = tempSpeed;}
	void SetMoveSpeed (const GLdouble &tempSpeed) {m_moveSpeed = tempSpeed;}

	// COLLSION DETECTION FUNCTIONS
	// set collision detection (TRUE = on)
	void SetCollisionDetectionOn (const bool &tempCol) {m_CollisionDetectionOn = tempCol;}
	// set number of bounding boxes
	void SetNoBoundingBoxes(const int & tempSize) {m_colDetect.SetNoBoundingBoxes(tempSize);}
	// set the co-ordinates of the world
	void SetWorldCoordinates (const GLdouble &tempX, const GLdouble &tempZ);
	// creates a linked list for each quadrant of the world and places the bounding box
	// data in each.  Then clears and deletes AABB array.
	void InitiateBoundingBoxes() {m_colDetect.CreateLinkedList();}

	// sets the co-ordinate of each plain
	void SetPlains (const int tempType,
				    const GLdouble tempXs, const GLdouble tempXe,
				    const GLdouble tempYs, const GLdouble tempYe,
				    const GLdouble tempZs, const GLdouble tempZe);

	//----------------------------------------------------------------------------------
	//  Get Methods
	//----------------------------------------------------------------------------------
	GLdouble GetLR () {return m_x;}
	GLdouble GetUD () {return m_y;}
	GLdouble GetFB () {return m_z;}	
	GLdouble GetAABBMaxX (const int & tempIndex) {return m_colDetect.GetAABBMaxX (tempIndex);}
	GLdouble GetAABBMinX (const int & tempIndex) {return m_colDetect.GetAABBMinX (tempIndex);}
	GLdouble GetAABBMaxY (const int & tempIndex) {return m_colDetect.GetAABBMaxY (tempIndex);}
	GLdouble GetAABBMinY (const int & tempIndex) {return m_colDetect.GetAABBMinY (tempIndex);}
	GLdouble GetAABBMaxZ (const int & tempIndex) {return m_colDetect.GetAABBMaxZ (tempIndex);}
	GLdouble GetAABBMinZ (const int & tempIndex) {return m_colDetect.GetAABBMinZ (tempIndex);}
	
	// position the camera
	void Position (GLdouble const & tempX,
				   GLdouble const & tempY,
				   GLdouble const & tempZ,
				   GLdouble const & tempAngle);

	// check whether ok to move
	void CheckCamera();

	// Used to pass direction to move or rotate  (i.e. 1, -1 or 0)
	void DirectionFB(int const & tempMove);
	void DirectionLR(int const & tempMove);
	void DirectionUD(int const & tempMove);
	void DirectionRotateLR(GLdouble const & tempMove);
	void DirectionLookUD(int const & tempMove);

	// display map
	void DisplayMap(const int & screenWidth, const int & screenHeight, 
		            const GLuint & tempImage);
	// display welcome screen
	void DisplayWelcomeScreen (const int & screenWidth, const int & screenHeight,
							   const int & tempExit, const GLuint & tempImage);
	// display no exit
	void DisplayNoExit (const int & screenWidth, const int & screenHeight, 
						const GLuint & tempImage);
	
private:

	//steep incline increments
	GLdouble m_incrementX;
	GLdouble m_incrementZ;
	int m_No_Plains;
	int m_plainNo;
	GLdouble m_plainHeight;

	// rotation variables
	GLdouble m_rotateAngleLR;
	GLdouble m_deltaAngleLR;
	GLdouble m_rotateAngleUD;	
	GLdouble m_deltaAngleUD;

	// movement variables
	GLdouble m_x, m_y, m_z, m_zLast, m_xLast;
	GLdouble m_lookX, m_lookY,m_lookZ;
	GLdouble m_lookXX, m_lookYY, m_lookZZ;
	GLdouble m_deltaMoveLR;
	GLdouble m_deltaMoveFB;
	GLdouble m_deltaMoveUD;
	GLdouble m_direction;

	// movement speed (step size)
	GLdouble m_rotateSpeed;
	GLdouble m_moveSpeed;
	
	// is it ok to move
	bool MoveFBOK();
	bool MoveLROK();
	bool MoveUDOK();
	bool RotateLROK();
	bool LookUDOK();

	// move around the world
	void MoveFB();
	void MoveLR();
	void MoveUD();
	void RotateLR();
	void LookUD();
	
	// overloaded function for setting plain
	void SetPlains(const int & moveX, const int & moveZ);

	// resets camera
	void ResetXYZ();
	// display new view
	void callGLLookAt();

	bool m_CollisionDetectionOn;

	// objects
	Collision m_colDetect;
	CameraMap m_map;
	PlainLinkedList m_Plain;

	// These functions were set up to climb stairs, but are not used.
	// The Plain object is used instead
	void ClimbSteps(GLdouble stepStart, GLdouble stepFinish, GLdouble stepHeight, GLdouble stepWidth, int noSteps);
	void CheckSteps();

	CEasySound *es;
	CSound* stepSound;

	//----------------------------------------------------------------------------------

    // Privatised copy constructor and assignment operator
    Camera (const Camera &cam) {};
    Camera &operator = (const Camera &cam) {};
};

#endif