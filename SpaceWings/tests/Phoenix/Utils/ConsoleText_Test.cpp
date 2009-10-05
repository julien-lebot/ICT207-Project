/*
*	ConsoleText_Test.cpp
*
*	Console Text Output test suite
*
*  Created by Julien Lebot on 04/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <boost/test/auto_unit_test.hpp>
#include <iostream>
#include <GL/glut.h>
#include <gltext.h>
#include <cstdarg>
#include <map>

using namespace gltext;

FontRendererPtr btmRenderer;
FontRendererPtr pxmRenderer;
FontRendererPtr texRenderer;
FontRendererPtr mipRenderer;

class FPSCounter
{
public:
	FPSCounter()
	{
		mFPS = 0;
		mFrameCount = 0;
		mElapsedTime = 0;
	}

	void update(float dt)
	{
		mElapsedTime += dt;
		++mFrameCount;
		if (mElapsedTime > 1.0f)
		{
			mFPS = mFrameCount;
			mFrameCount = 0;
			mElapsedTime -= 1.0f;
		}
	}

	int getFPS() const
	{
		return mFPS;
	}

private:
	int mFPS;
	int mFrameCount;
	float mElapsedTime;
};

struct Renderer
{
	Renderer(std::string n, const FontRendererPtr& r)
	{
		name = n;
		renderer = r;
	}

	std::string name;
	FontRendererPtr renderer;
};

std::vector<Renderer> gRenderers;
unsigned gCurrentRenderer;
int gLastUpdate;
FPSCounter gFPSCounter;
static const char* gStrings[] =
{
	"The five boxing wizards jump quickly.",
	"eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
};
static const unsigned gStringCount = sizeof(gStrings) / sizeof(*gStrings);
unsigned gCurrentString = 0;

void display()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	const std::string& name = gRenderers[gCurrentRenderer].name;
	const FontRendererPtr& renderer = gRenderers[gCurrentRenderer].renderer;

	int fps = gFPSCounter.getFPS();
	std::ostringstream ss;
	ss << "FPS: " << fps << "\n" << "Renderer: " << name << "\n\n";
	for (int i = 0; i < 12; ++i)
	{
		ss << gStrings[gCurrentString] << "\n";
	}

	renderer->render(ss.str().c_str());

	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
}

void keydown(unsigned char key, int x, int y)
{
	if (key == 27 || key == 'q')
	{
		exit(0);
	}
	else if (key == ' ')
	{
		gCurrentRenderer = (gCurrentRenderer + 1) % gRenderers.size();
	}
	else if (key == 'n')
	{
		gCurrentString = (gCurrentString + 1) % gStringCount;
	}
}

void idle()
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	float dt = (now - gLastUpdate) / 1000.0f;
	gLastUpdate = now;

	gFPSCounter.update(dt);
	glutPostRedisplay();
}

struct CinPolicy
{
	char getInput()
	{
		char c;
		std::cin >> c;
		return c;
	}
};

struct CoutPolicy
{
	void drawCharacter(int c)
	{
		std::cout << (char)c;
	}
};

void average(int count, ...)
{
	va_list ap;
	int j;
	double tot = 0;
	va_start(ap, count); //Requires the last fixed parameter (to get the address)
	for(j=0; j<count; j++)
		tot+=va_arg(ap, double); //Requires the type to cast to. Increments ap to the next argument.
	va_end(ap);
	std::cout << tot/count << std::endl;
}

void printout(int argc, ...)
{
	va_list va;

	va_start(va, argc);
	for (int j = 0; j < argc; ++j)
	{
		std::cout << va_arg(va, char *) << std::endl;
	}
	va_end(va);
}

void move(int argc, double amount)
{

}

typedef void (*fpointer)(int argc, ...);

std::string fontPath = "C:\\Windows\\Fonts\\";

int argc = 1;
char *argv = "program";

BOOST_AUTO_TEST_SUITE( Console_test_suite )
BOOST_AUTO_TEST_CASE( ConsoleText_test )
{
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);
	glutInit(&argc, &argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Alpha Example");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keydown);
	glutIdleFunc(idle);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FontPtr font = OpenFont((fontPath + "arial.ttf").c_str(), 26);
	BOOST_CHECK(font);

	btmRenderer = CreateRenderer(BITMAP, font);
	BOOST_CHECK(btmRenderer);

	pxmRenderer = CreateRenderer(PIXMAP, font);
	BOOST_CHECK(pxmRenderer);

	texRenderer = CreateRenderer(TEXTURE, font);
	BOOST_CHECK(texRenderer);

	mipRenderer = CreateRenderer(MIPMAP, font);
	BOOST_CHECK(mipRenderer);

	gRenderers.push_back(Renderer("bitmap",  btmRenderer));
	gRenderers.push_back(Renderer("pixmap",  pxmRenderer));
	gRenderers.push_back(Renderer("texture", texRenderer));
	gRenderers.push_back(Renderer("mipmap",  mipRenderer));
	gCurrentRenderer = 0;

	gLastUpdate = glutGet(GLUT_ELAPSED_TIME);

	//glutMainLoop();
}

BOOST_AUTO_TEST_SUITE_END()