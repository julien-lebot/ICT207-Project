/*
*	Resource_Test.cpp
*
*	Resource test suite
*
*  Created by Julien Lebot on 16/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <EngineFoundation/ResourceManagement/Resource.hpp>
#include <EngineFoundation/ResourceManagement/GLTextureManager.hpp>
#include <EngineFoundation/Renderer/Renderer.hpp>

namespace EngineFoundation
{
	class GLRenderer
	: public Renderer
	{
	public:
		void init()
		{
			mTextureManager = new GLTextureManager();
		}

		void check()
		{
			BOOST_CHECK_EQUAL(&TextureManager::instance(), mTextureManager);
		}

		void shutdown()
		{
			delete mTextureManager;
		}
	};
}

using namespace EngineFoundation;

BOOST_AUTO_TEST_SUITE( Resource_test_suite )
BOOST_AUTO_TEST_CASE( Resource_test )
{

}
BOOST_AUTO_TEST_CASE( TextureManager_test )
{
	GLRenderer r;
	r.init();
	r.check();
	r.shutdown();
}
BOOST_AUTO_TEST_CASE( ResourceHandleCreator_test )
{
	ResourceHandleCreator<int> c;
	int h1 = c.createHandle();
	int h2 = c.createHandle();
	int h3 = -5;
	int h4 = 10;
	BOOST_CHECK(!c.isValid(h3));
	BOOST_CHECK(!c.isValid(h4));
	BOOST_CHECK(h1 != h2);
	c.deleteHandle(h1);
	int h5 = c.createHandle();
	BOOST_CHECK(h5 == h1);
	c.purge();
	BOOST_CHECK(!c.isValid(h5));
}
BOOST_AUTO_TEST_SUITE_END()