#include <Phoenix/Base.h>
#include <Phoenix/Vector3.hpp>
#include <Phoenix/Root.hpp>
#include <Phoenix/Window.hpp>
#include <Phoenix/Camera.hpp>
#include <Phoenix/Tuple.hpp>
#include <Phoenix/StaticCheck.hpp>
#include <Phoenix/NetworkManager.hpp>
#include <Phoenix/GameLogicIntegrator.hpp>
#include <Phoenix/EntityManager.hpp>
#include <Phoenix/Input.hpp>
#include <Shay/Shay.h>
#include <fstream>

using namespace Phoenix;
using namespace Phoenix::Math;

void setCamera(const Vector3f &pos,
			   const Vector3f &target)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos.x, pos.y, pos.z,
			  target.x, target.y, target.z,
			  0, 1, 0);
}

class DemoWindow
	: public Window
{
	Phoenix::NetworkManager networkmanager;
	EntityManager entitymanager;
	Shay::ShayWorld s;
	Phoenix::Camera c;
	GameLogicIntegrator gamelogicintegrator;
	bool mSwitchWorld;
public:
	DemoWindow(const std::string& name,
		const Resolution_us& res,
		bool fullscreen)
		: Window(NULL, name, res, fullscreen),
		mSwitchWorld(false)
	{
	}
	void onDisplay()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(&c.getProjectionMatrix()[0][0]);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(&c.getViewMatrix()[0][0]);
		
		//atm.Update(c);

		//glClearColor(97.0/255.0, 140.0/255.0, 185.0/255.0, 1.0);
		glPushMatrix();
		s.DrawBackdrop();
		glPopMatrix();
		
		entitymanager.renderAll();

		//static_cast<Texture*>(texture.get())->bind(0);
		//Renderer::instance().render(testEntity.getRenderOperation());

		//static_cast<Texture*>(texture2.get())->bind(0);
		//glTranslatef(0,0,-20);
		//Renderer::instance().render(testEntity.getRenderOperation());
		//static_cast<Texture*>(texture.get())->unbind();

		glutSwapBuffers();
	}

	void onIdle()
	{
		if(networkmanager.isServer() && networkmanager.isConnected())
		{
			std::vector<Network::Packet>  list;
			entitymanager.createConstructList(list);
			networkmanager.sendPackets(list);
		}
		networkmanager.send_recv();
		networkmanager.processIncoming(entitymanager);
		gamelogicintegrator.logicLoop(entitymanager.getEntityListRef());
		if(gamelogicintegrator.timeToRender())
			glutPostRedisplay();
	}

	void onReshape(int w, int h)
	{
		glViewport(0,0,static_cast<GLsizei>(w), static_cast<GLsizei>(h));
		float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		c.perspective(60, aspectRatio, 1.0, 100000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void onMotion(int x, int y)
	{
		Input inp;
		if(networkmanager.isServer())
			inp = gamelogicintegrator.getServerInput();
		else
			inp = gamelogicintegrator.getClientInput();
		
		inp.deltaX = abs(inp.previousX - x);
		inp.deltaX = abs(inp.previousY - y);
		inp.previousX = x;
		inp.previousY = y;

		if(networkmanager.isServer())
			gamelogicintegrator.setServerInput(inp);
		else
			networkmanager.sendinput(inp);
	}

	void onKeyboardUp(unsigned char key, int x, int y)
	{
		Input inp;
		switch (key)
		{
			case 'w':
				inp.accelerate = false;				
				break;
			case 's':
				inp.breaks = false;
				break;
			case 'a':
				inp.rollLeft = false;
				break;
			case 'd':
				inp.rollRight = false;
				break;
		}

		if(networkmanager.isServer())
			gamelogicintegrator.setServerInput(inp);
		else
			networkmanager.sendinput(inp);
	}

	void onKeyboard(unsigned char key, int x, int y)
	{
		Input inp;
		switch (key)
		{
			case 'w':
				inp.accelerate = true;				
				break;
			case 's':
				inp.breaks = true;
				break;
			case 'a':
				inp.rollLeft = true;
				break;
			case 'd':
				inp.rollRight = true;
				break;
		}

		if(networkmanager.isServer())
			gamelogicintegrator.setServerInput(inp);
		else
			networkmanager.sendinput(inp);
		
		/*float factor = 1.0f;
		if (mSwitchWorld)
			factor = 10.0f;
		else
			factor = 500.0f;

		switch (key)
		{

		case 'w':	//Forward
			c.move(0,0,factor);
			break;
		case 's':	//Backward
			c.move(0,0,-factor);
			break;
		case 'a':	//Strafe left
			c.move(-factor,0,0);
			break;
		case 'd':	//Strafe right
			c.move(factor,0,0);
			break;

		case 'i':	//Rotate up
			c.rotate(0,M_PI/180.0,0);
			break;
		case 'k':	//Rotate down
			c.rotate(0,-M_PI/180.0,0);
			break;
		case 'j':	//Rotate left
			c.rotate(M_PI/180.0,0,0);
			break;
		case 'l':	//Rotate right
			c.rotate(-M_PI/180.0,0,0);
			break;

		case 'p':	//Print current coordinates
			std::cout << c.getPosition() << std::endl;
			break;

		case 'f':	//Switch worlds
			if (!mSwitchWorld)
			{
				//c.move(Vector3f(0.0, 0.0, -25.0));
				c.setPosition(Vector3f(0.0, 0.0, -150.0));
				//c.setDirection(Vector3f(0.0, 0.0, 0.0));
			}
			else
			{
				c.setPosition(Vector3f(4000.0, 10450.0, 20620.0));
				//c.setDirection(Vector3f(3999.0, 10450.0, 20620.0));
			}
			mSwitchWorld = !mSwitchWorld;
			break;
		};
		*/
	}

protected:

	void initializeImpl()
	{
		entitymanager.setup (networkmanager.isServer());

		c.setPosition (Vector3f (0.0f, 20000.0f, 0.0f));
		c.rotate(0.0, -PI / 2.0, 0.0);
		//c.lookAt (Vector3f (0.0f, 0.0f, 0.0f));

		/*
		float lightAmbient[] = { 0.2f, 0.3f, 0.6f, 1.0f };
		float lightDiffuse[] = { 0.2f, 0.3f, 0.6f, 1.0f };

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		*/

		//testEntity.load(swmR);
		
		glShadeModel(GL_SMOOTH);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		
		//c.move(Vector3f(0.0, 10450.0, 0.0));
		//c.setPosition(Vector3f(4000.0, 10450.0, 20620.0));
		//c.setDirection(Vector3f(3999.0, 10450.0, 20620.0));
		//c.rotate(Vector3f::Y, static_cast<Math::Units::Radians>(270 * Math::Units::degrees));

		//atm.Start();
		s.myinit();

		glClearColor(0, 0, 0, 1.0);
	}
};

int main(int argc, char *argv[])
{
	Root::instance().initialize();
	Root::instance().addWindow(WindowPtr(new DemoWindow("Demo !", Resolution_us(800, 600), false)));
	Root::instance().setIdle2ActiveWindow();
	Root::instance().setIdle(true);
	Root::instance().enterMainLoop();

	return EXIT_SUCCESS;
}