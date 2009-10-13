/*
*	Renderer.hpp
*
*	Abstract renderer class
*
*  Created by Julien Lebot on 07/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Renderer_hpp__
#define __Renderer_hpp__

#include <Phoenix/Base.h>
#include <Phoenix/Color.hpp>
#include <Phoenix/ConfigOpts.hpp>
#include <Phoenix/RendererCommon.hpp>
#include <Phoenix/RenderOperation.hpp>
#include <Phoenix/Singleton.hpp>
#include <string>

namespace Phoenix
{
	class TextureManager;
	class HardwareBufferManager;

	class _PhoenixExport Renderer
	: public Singleton<Renderer>
	{
		friend struct CreateUsingNew<Renderer>;
	private:
		bool mStopRendering;

#define MAX_LIGHTS 8

	protected:
		Renderer();
		virtual ~Renderer() {}

	public:

		void setConfigOption(const std::string &name, const ConfigOption &option);

		void setAmbientLight(float r, float g, float b);

		void setShadingType(ShadingType st);
	
		void setLightingEnabled(bool enabled);

		std::string getErrorDescription() const;

		void setnormalizeNormals(bool normalize);

		void setViewport();

		void beginFrame();

		void render(const RenderOperation &rop);

		void endFrame();

		void enterOrthographicMode(GLint winWidth = 0, GLint winHeight = 0);

		void leaveOrthographicMode();

		void setFog(FogMode mode, const Color& color, float density, float start, float end);

	protected:
		TextureManager* mTextureManager;
		ConfigOptionMap mOptions;
	};
}

#endif // __Renderer_hpp__