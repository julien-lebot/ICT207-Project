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
//#include <Phoenix/TextureManager.hpp>
#include <Phoenix/BufferElementGroup.hpp>
#include <Phoenix/NonCopyable.hpp>

using namespace Phoenix;

// Instantiate singleton's specialized instance variables
template<> Renderer* Singleton<Phoenix::Renderer>::mInstance = NULL;
template<> bool Singleton<Phoenix::Renderer>::mDestroyed = false;

#if defined(_THREAD_SUPPORT)
template<> boost::recursive_mutex Singleton<Phoenix::Renderer>::singletonMutex;
#endif

unsigned short getTypeCount(BufferElementType etype)
{
	switch (etype)
	{
	case COLOUR:
		return 1;
	case FLOAT1:
		return 1;
	case FLOAT2:
		return 2;
	case FLOAT3:
		return 3;
	case FLOAT4:
		return 4;
	case SHORT1:
		return 1;
	case SHORT2:
		return 2;
	case SHORT3:
		return 3;
	case SHORT4:
		return 4;
	case UBYTE4:
		return 4;
	default:
		return 3;
	}
}

GLenum getGLType(unsigned int type)
{
	switch(type)
	{
	case FLOAT1:
	case FLOAT2:
	case FLOAT3:
	case FLOAT4:
		return GL_FLOAT;
	case SHORT1:
	case SHORT2:
	case SHORT3:
	case SHORT4:
		return GL_SHORT;
	case COLOUR:
	case UBYTE4:
		return GL_UNSIGNED_BYTE;
	default:
		return 0;
	};
}

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

std::string Renderer::getErrorDescription() const
{	
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		return std::string((char*)errString);
	}
	else
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

void Renderer::render(const RenderOperation &rop)
{
	void* buffer = 0;

	const BufferElementGroup::ElementList& elmtList = rop.vertexData->bufferElementGroup.getElements();
	BufferElementGroup::ElementList::const_iterator elem, elemEnd;
	elemEnd = elmtList.end();

	for (elem = elmtList.begin(); elem != elemEnd; ++elem)
	{
		HardwareBufferPtr vertexBuffer = rop.vertexData->bufferBinding[elem->getSource()];
		assert(vertexBuffer != NULL);

		if(1) // Use VBO
		{
			vertexBuffer->bind();
			buffer = BUFFER_OFFSET(elem->getOffset());
		}

		// fixed-function & built in attribute support
		switch(elem->getSemantic())
		{
		case POSITION:
			glVertexPointer(getTypeCount(elem->getType()), getGLType(elem->getType()), 0, buffer);
			glEnableClientState(GL_VERTEX_ARRAY);
			break;
		case NORMAL:
			glNormalPointer(getGLType(elem->getType()),	0, buffer);
			glEnableClientState(GL_NORMAL_ARRAY);
			break;
		case DIFFUSE:
			glColorPointer(4, 
				getGLType(elem->getType()), 0, buffer);
			glEnableClientState(GL_COLOR_ARRAY);
			break;
		case SPECULAR:
			if (GLEE_EXT_secondary_color)
			{
				glSecondaryColorPointerEXT(4, 
					getGLType(elem->getType()), 0, buffer);
				glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
			}
			break;
		case TEXTURE_COORDINATES:
			/*
			if (0)
			{
				// Programmable pipeline - direct UV assignment
				glClientActiveTextureARB(GL_TEXTURE0 + elem->getIndex());
				glTexCoordPointer(
				VertexElement::getTypeCount(elem->getType()), 
				GLHardwareBufferManager::getGLType(elem->getType()),
				static_cast<GLsizei>(vertexBuffer->getVertexSize()), 
				pBufferData);
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			}
			else*/
			{
				// fixed function matching to units based on tex_coord_set
				//for (int i = 0; i < mDisabledTexUnitsFrom; i++)
				{
					// Only set this texture unit's texcoord pointer if it
					// is supposed to be using this element's index
					//if (mTextureCoordIndex[i] == elem->getIndex() && i < mFixedFunctionTextureUnits)
					{
						//if (multitexturing)
						//	glClientActiveTextureARB(GL_TEXTURE0 + i);
						glTexCoordPointer(getTypeCount(elem->getType()), getGLType(elem->getType()), 0, buffer);
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					}
				}
			}
			break;
		default:
			break;
		};
	}

	// Find the correct type to render
	GLint primType;

	switch (rop.primitiveType)
	{
	case RenderOperation::POINTS:
		primType = GL_POINTS;
		break;
	case RenderOperation::LINES:
		primType = GL_LINES;
		break;
	case RenderOperation::LINE_STRIP:
		primType = GL_LINE_STRIP;
		break;
	default:
	case RenderOperation::TRIANGLES:
		primType = GL_TRIANGLES;
		break;
	case RenderOperation::TRIANGLE_STRIP:
		primType = GL_TRIANGLE_STRIP;
		break;
	case RenderOperation::TRIANGLE_FAN:
		primType = GL_TRIANGLE_FAN;
		break;
	}

	if (rop.indexed)
	{
		if(1) // Use VBO
		{
			rop.indexData->indexBuffer->bind();
			buffer = BUFFER_OFFSET(rop.indexData->start * rop.indexData->indexBuffer->getSize());
		}

		GLenum indexType = GL_UNSIGNED_INT /*: GL_UNSIGNED_SHORT GL_UNSIGNED_INT*/;

		do
		{
			glDrawElements(primType, rop.indexData->count, indexType, buffer);
		} while (0);

	}
	else
	{
		do
		{
			// rop.vertexData->start
			std::vector<int>::const_iterator offsetIter = rop.mtlOffsets.begin ();					//Added by Terje
			std::vector<std::string>::const_iterator mtlOffsetIter = rop.mtlOffsetNames.begin ();	//Added by Terje
			std::vector<Phoenix::Material>::const_iterator mtlIter;									//Added by Terje
		
			while ( offsetIter != rop.mtlOffsets.end () )											//While loop added by Terje
			{
				mtlIter = rop.mtlList.begin();
				while((*mtlIter).getName() != *mtlOffsetIter && mtlIter != rop.mtlList.end())
				{
					mtlIter++;
				}
				//static_cast<Texture*>(texture.get())->bind(0);
				static_cast<Texture*>((*mtlIter).getDiffuseTexture().get())->bind(0);
				int offset1 = *(offsetIter++);
				int offset2 = *(offsetIter++);
				
				glDrawArrays (primType,offset1, offset2 );
				mtlOffsetIter++;
			}
			
			//glDrawArrays(primType, 0, rop.vertexData->count);
		} while (0);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	// only valid up to GL_MAX_TEXTURE_COORDS, which is recorded in mFixedFunctionTextureUnits
	/*if (0) // multitexturing
	{
	for (int i = 0; i < mFixedFunctionTextureUnits; i++)
	{
	glClientActiveTextureARB(GL_TEXTURE0 + i);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	glClientActiveTextureARB(GL_TEXTURE0);
	}
	else*/
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	if (GLEE_EXT_secondary_color)
	{
		glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
	}
	// unbind any custom attributes
	/*for (std::vector<GLuint>::iterator ai = attribsBound.begin(); ai != attribsBound.end(); ++ai)
	{
	glDisableVertexAttribArrayARB(*ai); 
	}*/

	glColor4f(1,1,1,1);
	if (GLEE_EXT_secondary_color)
	{
		glSecondaryColor3fEXT(0.0f, 0.0f, 0.0f);
	}
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
