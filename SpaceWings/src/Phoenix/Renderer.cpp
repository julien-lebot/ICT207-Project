/*
*	Renderer.cpp
*
*	OpenGL renderer class
*
*  Created by Julien Lebot on 07/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/Renderer.hpp>
#include <Phoenix/Resolution.hpp>
#include <Phoenix/TextureManager.hpp>
#include <Phoenix/NonCopyable.hpp>
#include <GL/glut.h>

using namespace Phoenix;

Renderer::Renderer()
{
	ConfigOption fullscreen_opt(false);
	fullscreen_opt.addPossibleValue(true).addPossibleValue(false);
	mOptions["fullscreen"] = fullscreen_opt;

	ConfigOption resolution_opt(Resolution_us(640, 480));
	mOptions["resolution"] = resolution_opt;
}

void Renderer::setConfigOption(const std::string &name, const ConfigOption &option)
{
	mOptions[name] = option;
}

void Renderer::initialize(bool autoCreateWindow, const std::string& windowTitle)
{

}

void Renderer::reinitialize()
{
}

void Renderer::shutdown()
{

}

void Renderer::setAmbientLight(float r, float g, float b)
{
	
}

void Renderer::setShadingType(ShadingType st)
{
	switch(st)
	{
	case FLAT_SHADING:
		glShadeModel(GL_FLAT);
		break;
	default:
		glShadeModel(GL_SMOOTH);
		break;
	}
}

void Renderer::setLightingEnabled(bool enabled)
{
	if (enabled) 
	{      
		glEnable(GL_LIGHTING);
	} 
	else 
	{
		glDisable(GL_LIGHTING);
	}
}

std::string Renderer::getErrorDescription(/*long errorNumber*/) const
{
	return "";
}

void Renderer::setnormalizeNormals(bool normalize)
{
	if (normalize)
		glEnable(GL_NORMALIZE);
	else
		glDisable(GL_NORMALIZE);
}

void Renderer::setViewport()
{

}

void Renderer::beginFrame()
{

}

void Renderer::render()
{

}

void Renderer::endFrame()
{

}

void Renderer::enterOrthographicMode(GLint winWidth, GLint winHeight)
{
	GLint viewport[4];

	if(winWidth <= 0 || winHeight <= 0)
	{
		glGetIntegerv(GL_VIEWPORT, viewport);
		winWidth  = viewport[2];
		winHeight = viewport[3];
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, winWidth, winHeight, 0);
	glDisable(GL_DEPTH_TEST);
}

void Renderer::leaveOrthographicMode()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Renderer::setFog(FogMode mode, const Color& color, float density, float start, float end)
{
	GLint fogMode;
	switch (mode)
	{
	case FOG_EXP:
		fogMode = GL_EXP;
		break;
	case FOG_EXP2:
		fogMode = GL_EXP2;
		break;
	case FOG_LINEAR:
		fogMode = GL_LINEAR;
		break;
	default:
		glDisable(GL_FOG);
		return;
	}

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, fogMode);
	GLfloat fogColor[4] = {color.r, color.g, color.b, color.a};
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, density);
	glFogf(GL_FOG_START, start);
	glFogf(GL_FOG_END, end);
}
