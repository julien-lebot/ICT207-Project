//  camera.cpp
//
//  Implementation file for Camera Class
//  Defines all the methods declared, but not defined, in camera.h
//
//  Shay Leary, March 2005
//--------------------------------------------------------------------------------------
#include <Phoenix/Math.hpp>
#include <Shay/Camera.h>

#include <math.h>
#if defined(__GNUC__)
#include <GLUT/GLUT.h>
#else
#include <gl/glut.h>
#endif

//--------------------------------------------------------------------------------------
// Set initial values
//--------------------------------------------------------------------------------------
Camera::Camera()
{
	m_rotateSpeed = 0.0f;
	m_moveSpeed = 0.0f;

	ResetXYZ();

	m_deltaMoveFB = 0;
	m_deltaMoveLR = 0;
	m_deltaMoveUD = 0;

	m_rotateAngleLR = 0.0;
	m_rotateAngleUD = 0.0;
	m_deltaAngleLR = 0.0;
	m_deltaAngleUD = 0.0;

	m_CollisionDetectionOn = true;

	// sound objects
//es = CEasySound::Instance();
	//stepSound = es->GetSound(es->Load("sounds/step.wav"));
}

//--------------------------------------------------------------------------------------
// Reset camera values
//--------------------------------------------------------------------------------------
void Camera::ResetXYZ()
{
	m_x = 0.0f;
	m_y = 1.75f;
	m_z = 0.0f;
	
	m_lookX = 0.0f;
	m_lookY = 0.0f;
	m_lookZ = -1.0f;
	
	m_lookXX = 1.0f;
	m_lookYY = 1.0f;
	m_lookZZ = 0.0f;
}

//--------------------------------------------------------------------------------------
//  Determine direction
//--------------------------------------------------------------------------------------
void Camera::DirectionFB(int const & tempMove)
{
	m_deltaMoveFB = tempMove;
}
//--------------------------------------------------------------------------------------
void Camera::DirectionLR(int const & tempMove)
{
	m_deltaMoveLR = tempMove;
}
//--------------------------------------------------------------------------------------
// Not used but allows up and don movement
void Camera::DirectionUD(int const & tempMove)
{
	m_deltaMoveUD = tempMove;
}

//--------------------------------------------------------------------------------------
void Camera::DirectionRotateLR(GLdouble const & tempMove)
{
	m_deltaAngleLR = tempMove * m_rotateSpeed;
}

//--------------------------------------------------------------------------------------
void Camera::DirectionLookUD(int const & tempMove)
{
	m_deltaAngleUD = tempMove * m_rotateSpeed;
}


//--------------------------------------------------------------------------------------
// Is ok to move camera backwards and forwards
//--------------------------------------------------------------------------------------
bool Camera::MoveFBOK()
{
	bool tempReturn = false;
	if (m_deltaMoveFB < 0 || m_deltaMoveFB > 0)
	{
		tempReturn = true;
	}

	return tempReturn;
}

//--------------------------------------------------------------------------------------
// Is ok to move camera sideways
//--------------------------------------------------------------------------------------
bool Camera::MoveLROK()
{
	bool tempReturn = false;
	if (m_deltaMoveLR < 0 || m_deltaMoveLR > 0)
	{
		tempReturn = true;
	}

	return tempReturn;
}

//--------------------------------------------------------------------------------------
// Is ok to move camera up and down (not used)
//--------------------------------------------------------------------------------------
bool Camera::MoveUDOK()
{
	bool tempReturn = false;
	if (m_deltaMoveUD < 0 || m_deltaMoveUD > 0)
	{
		tempReturn = true;
	}
	return tempReturn;
}

//--------------------------------------------------------------------------------------
// Is ok to rotate sideways
//--------------------------------------------------------------------------------------
bool Camera::RotateLROK()
{
	bool tempReturn = false;
	if ((m_deltaAngleLR/m_rotateSpeed) < 0 || (m_deltaAngleLR/m_rotateSpeed) > 0)
	{
		tempReturn = true;
	}

	return tempReturn;
}

//--------------------------------------------------------------------------------------
// Is ok to rotate up and down
//--------------------------------------------------------------------------------------
bool Camera::LookUDOK()
{
	bool tempReturn = false;
	if ((m_deltaAngleUD/m_rotateSpeed) < 0 || (m_deltaAngleUD/m_rotateSpeed) > 0)
	{
		tempReturn = true;
	}

	return tempReturn;
}

//--------------------------------------------------------------------------------------
// Move camera backwards and forwards
//--------------------------------------------------------------------------------------
void Camera::MoveFB()
{
	// record previous co-ordinates
	m_xLast = m_x;
	m_zLast = m_z;

	// set movement step
	GLdouble moveZ = (m_deltaMoveFB * (m_lookZ) * m_moveSpeed);
	GLdouble moveX = (m_deltaMoveFB * (m_lookX) * m_moveSpeed);

	if (m_CollisionDetectionOn)
	{
		GLdouble startX = m_x + moveX * 5.0;
		GLdouble startZ = m_z + moveZ * 5.0;

		// check if collsion
		if (!(m_colDetect.Collide(startX + m_lookX, m_y + m_lookY, startZ + m_lookZ)))
		{
			// increment position
			m_x += moveX;
			m_z += moveZ;
			// check plain
			SetPlains(moveX, moveZ);
			// redisplay
			callGLLookAt();
		}
	}
	else
	{
		// increment position
		m_x += moveX;
		m_z += moveZ;
		// check plain
		SetPlains(moveX, moveZ);
		// redisplay
		callGLLookAt();
	}	
}

//--------------------------------------------------------------------------------------
// Move camera left and right (sideways)
//--------------------------------------------------------------------------------------
void Camera::MoveLR()
{
	// record previous co-ordinates
	m_zLast = m_z;
	m_xLast = m_x;

	// set movement step
	GLdouble moveZ = (m_deltaMoveLR * (m_lookZZ) * m_moveSpeed);
	GLdouble moveX = (m_deltaMoveLR * (m_lookXX) * m_moveSpeed);

	if (m_CollisionDetectionOn)
	{
		GLdouble startX = m_x + moveX * 1.0;
		GLdouble startZ = m_z + moveZ * m_moveSpeed * 1.0;

		// check if collsion
		if (!(m_colDetect.Collide(startX + m_lookXX, m_y + m_lookYY, startZ + m_lookZZ)))
		{
			// increment position
			m_x += moveX;
			m_z += moveZ;
			// check plain
			SetPlains(moveX, moveZ);
			// redisplay
			callGLLookAt();
		}
	}
	else
	{
		// increment position
		m_x += moveX;
		m_z += moveZ;
		// check plain
		SetPlains(moveX, moveZ);
		// redisplay
		callGLLookAt();
	}
}

//----------------------------------------------------------------------------------------
//  Places camera at the correct level on the current plain
//----------------------------------------------------------------------------------------
void Camera::SetPlains(const int & moveX, const int & moveZ)
{
	GLdouble y_max = 0, y_temp = 0;

	// store number of plains (stops from looping through linked list each time)
	if (m_No_Plains == 0) m_No_Plains = m_Plain.GetListSize();

	for (int i = 0;  i < m_No_Plains; i++)
	{
		// if camera is positioned on a plain
		if ((m_z <= m_Plain.GetZend(i)) && (m_z >= m_Plain.GetZstart(i)) && (m_x <= m_Plain.GetXend(i)) && (m_x >= m_Plain.GetXstart(i)))
		{
			// flat plain
			y_temp = m_Plain.GetYstart(i);
			
			// if plain slopes in x direction
			if (m_Plain.GetType(i) == 1)
				y_temp += (m_x - m_Plain.GetXstart(i)) * (m_Plain.GetYend(i) - m_Plain.GetYstart(i)) / (m_Plain.GetXend(i) - m_Plain.GetXstart(i));

			// if plain slopes in z direction
			if (m_Plain.GetType(i) == 2)
				y_temp += (m_z - m_Plain.GetZstart(i)) * (m_Plain.GetYend(i) - m_Plain.GetYstart(i)) / (m_Plain.GetZend(i) - m_Plain.GetZstart(i));
			
			if (y_max < y_temp)
				y_max = y_temp;
		}
	}

	if (y_max != 0)
		m_y = y_max;
}

//----------------------------------------------------------------------------------------
// Moves camera up and down (NOT USED)
//----------------------------------------------------------------------------------------
void Camera::MoveUD()
{
	if (m_CollisionDetectionOn)
	{
		GLdouble startY = m_y + m_deltaMoveUD * (m_lookYY) * m_moveSpeed * 5.0;

		if (!(m_colDetect.Collide(m_x + m_lookXX, startY + m_lookYY, m_z + m_lookZZ)))
		{
			m_y += m_deltaMoveUD * (m_lookYY) * m_moveSpeed;
			callGLLookAt();
		}
	}
	else
	{
		m_y += m_deltaMoveUD * (m_lookYY) * m_moveSpeed;
		callGLLookAt();
	}
}

//----------------------------------------------------------------------------------------
// Rotates camera left and right
//----------------------------------------------------------------------------------------
void Camera::RotateLR()
{
	m_rotateAngleLR += m_deltaAngleLR;
	m_lookX = sin(m_rotateAngleLR);
	m_lookZ = -cos(m_rotateAngleLR);
	m_lookXX = sin(m_rotateAngleLR + (float) M_PI/2.0);
	m_lookZZ = -cos(m_rotateAngleLR + (float) M_PI/2.0);
	callGLLookAt();
}

//----------------------------------------------------------------------------------------
//  Rotates camera up and down
//----------------------------------------------------------------------------------------
void Camera::LookUD()
{
	m_rotateAngleUD += m_deltaAngleUD;
	m_lookY = sin(m_rotateAngleUD);
	callGLLookAt();
}

//----------------------------------------------------------------------------------------
// Positions camera at co-ordinates of parameters
//----------------------------------------------------------------------------------------
void Camera::Position (GLdouble const & tempX, GLdouble const & tempY,
			           GLdouble const & tempZ, GLdouble const & tempAngle)
{
	ResetXYZ();
	
	m_x = tempX;
	m_y = tempY;
	m_z = tempZ;

	// rotate to correct angle
	m_rotateAngleLR = tempAngle * (M_PI / 180);
	m_lookX = sin(m_rotateAngleLR);
	m_lookZ = -cos(m_rotateAngleLR);
	m_lookXX = sin(m_rotateAngleLR + (float) M_PI/2.0);
	m_lookZZ = -cos(m_rotateAngleLR + (float) M_PI/2.0);
	m_rotateAngleUD = 0.0;
	m_deltaAngleUD = 0.0;

	// redislay
	callGLLookAt();
}

//----------------------------------------------------------------------------------------
// Check ok to move
//----------------------------------------------------------------------------------------
void Camera::CheckCamera()
{
	if (MoveFBOK()) MoveFB();
	if (MoveLROK()) MoveLR();
	if (MoveUDOK()) MoveUD();
	if (RotateLROK()) RotateLR();
	if (LookUDOK()) LookUD();
}


//----------------------------------------------------------------------------------------
//  Redisplay new camera view
//----------------------------------------------------------------------------------------
void Camera::callGLLookAt()
{
	glLoadIdentity();
	gluLookAt(m_x, m_y, m_z, 
		      m_x + m_lookX, m_y + m_lookY, m_z + m_lookZ,
			  0.0f, 1.0f, 0.0f);
}

//--------------------------------------------------------------------------------------
// Display map of world
//----------------------------------------------------------------------------------------

void Camera::DisplayMap(const int & screenWidth, const int & screenHeight, 
		                const GLuint & tempImage)
{
	m_map.DisplayMap(screenWidth, screenHeight, GetLR (), GetFB (), tempImage);
}

//--------------------------------------------------------------------------------------
// Display welcome or exit page
//----------------------------------------------------------------------------------------

void Camera::DisplayWelcomeScreen(const int & screenWidth, const int & screenHeight, 
		                const int & tempExit, const GLuint & tempImage)
{
	 m_map.DisplayWelcomeScreen(screenWidth, screenHeight, tempExit, tempImage);
}

//--------------------------------------------------------------------------------------
// Display welcome or exit page
//----------------------------------------------------------------------------------------

void Camera::DisplayNoExit(const int & screenWidth, const int & screenHeight, 
		                   const GLuint & tempImage)
{
	 m_map.DisplayNoExit(screenWidth, screenHeight, tempImage);
}

//----------------------------------------------------------------------------------------

void Camera::SetWorldCoordinates (const GLdouble &tempX, const GLdouble &tempZ)
{
	m_colDetect.SetWorldX(tempX);
	m_colDetect.SetWorldZ(tempZ);
}

//----------------------------------------------------------------------------------------

void Camera::SetPlains (const int tempType,
				        const GLdouble tempXs, const GLdouble tempXe,
				        const GLdouble tempYs, const GLdouble tempYe,
				        const GLdouble tempZs, const GLdouble tempZe)
{
	m_Plain.AddToStart(tempType, tempXs, tempXe, tempYs, tempYe, tempZs, tempZe);
}