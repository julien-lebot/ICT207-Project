#include <Phoenix/Base.h>
#include <Phoenix/HardwareBuffer.hpp>
#include <Phoenix/Vector3.hpp>
#include <Phoenix/Root.hpp>
#include <Phoenix/Window.hpp>
#include <Phoenix/Camera.hpp>
#include <Phoenix/Factory.hpp>
#include <Phoenix/PlayerEntity.hpp>
#include <Phoenix/Model.hpp>
#include <Phoenix/Texture.hpp>
#include <Shay/Shay.h>
#include <fstream>

#include <list>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

enum BufferElementSemantic
{
	POSITION = 1,
	//BLEND_WEIGHTS = 2,
	//BLEND_INDICES = 3,
	NORMAL = 4,
	DIFFUSE = 5,
	SPECULAR = 6,
	TEXTURE_COORDINATES = 7,
	//BINORMAL = 8,
	//TANGENT = 9
};

enum BufferElementType
{
	FLOAT1,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	COLOUR,
	SHORT1,
	SHORT2,
	SHORT3,
	SHORT4,
	UBYTE4
};

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

template <typename T>
struct glTraits {};

template<>
struct glTraits<signed char>
{
	enum {GL_TYPE = GL_BYTE};
};

template<>
struct glTraits<unsigned char>
{
	enum {GL_TYPE = GL_UNSIGNED_BYTE};
};

template<>
struct glTraits<short>
{
	enum {GL_TYPE = GL_SHORT};
};

template<>
struct glTraits<unsigned short>
{
	enum {GL_TYPE = GL_UNSIGNED_SHORT};
};

template<>
struct glTraits<int>
{
	enum {GL_TYPE = GL_INT};
};

template<>
struct glTraits<unsigned int>
{
	enum {GL_TYPE = GL_UNSIGNED_INT};
};

template<>
struct glTraits<float>
{
	enum {GL_TYPE = GL_FLOAT};
};

template<>
struct glTraits<double>
{ 
	enum {GL_TYPE = GL_DOUBLE};
};

template <class T>
void gpuVertexPointer(int size, const int stride = 0, const T* pointer = NULL)
{
	glVertexPointer(size, glTraits<T>::GL_TYPE, stride, pointer);
};

template <class T>
void gpuColorPointer(const int size, const int stride = 0, const T* pointer = NULL)
{
	glColorPointer(size, glTraits<T>::GL_TYPE, stride, pointer);
};

template <class T>
void gpuSecondaryColorPointer(const int size, const int stride = 0, const T* pointer = NULL)
{
	glSecondaryColorPointer(size, glTraits<T>::GL_TYPE, stride, pointer);
};

template <class T>
void gpuTexCoordPointer(const int size, const int stride = 0, const T* pointer = NULL)
{
	glTexCoordPointer(size, glTraits<T>::GL_TYPE, stride, pointer);
};

template <class T>
void gpuIndexPointer(const int stride = 0, const T* pointer = NULL)
{
	glIndexPointer(glTraits<T>::GL_TYPE, stride, pointer);
};

template <class T>
void gpuNormalPointer(const int stride = 0, const T* pointer = NULL)
{
	glNormalPointer(glTraits<T>::GL_TYPE, stride, pointer);
};

template <class T>
void gpuFogCoordPointer(const int stride = 0, const T* pointer = NULL)
{
	glFogCoordPointer(glTraits<T>::GL_TYPE, stride, pointer);
};

//typedef std::set<HardwareIndexBuffer*> IndexBufferList;

using namespace Phoenix;
using namespace Phoenix::Math;

class BufferElement
{
public:
	BufferElement(unsigned short source,
				  std::size_t offset,
				  BufferElementSemantic semantic,
				  BufferElementType type,
				  unsigned short index=0)
				  : mSourceID(source), mIndex(index), mOffset(offset), mSemantic(semantic), mType(type)
	{}

	unsigned short getSource() const { return mSourceID; }
	unsigned short getIndex() const { return mIndex; }
	std::size_t getOffset() const { return mOffset; }
	BufferElementSemantic getSemantic() const { return mSemantic; }
	BufferElementType getType() const { return mType; }

protected:
	unsigned short mSourceID, mIndex;
	std::size_t mOffset;
	BufferElementSemantic mSemantic;
	BufferElementType mType;
};

class BufferElementGroup
{
public:
	typedef std::list<BufferElement> ElementList;

	void addElement(unsigned short source,
					std::size_t offset,
					BufferElementSemantic semantic,
					BufferElementType type,
					unsigned short index=0)
	{
		mElementList.push_back(BufferElement(source, offset, semantic, type, index));
	}

	const ElementList& getElements() { return mElementList; }
protected:
	ElementList mElementList;
};

struct VertexData
{
	std::size_t start, count;
	BufferElementGroup *bufferElementGroup;

	VertexData()
		: start(0),
		  count(0)
	{
		bufferElementGroup = new BufferElementGroup();
	}

	~VertexData()
	{
		delete bufferElementGroup;
	}
};

struct IndexData
{
	std::size_t start, count;
	HardwareBuffer* indexBuffer;	// make me shared_ptr
};

struct RenderOperation
{
	enum PrimitiveType
	{
	  POINTS,
	  LINES,
	  LINE_STRIP,
	  TRIANGLES,
	  TRIANGLE_STRIP,
	  TRIANGLE_FAN
	};

	VertexData *vertexData;
	IndexData *indexData;
	PrimitiveType primitiveType;
	bool indexed;
};

typedef std::map<unsigned short, HardwareBuffer*> VertexBufferList;
VertexBufferList vbl;

struct Renderer
{
	void render(const RenderOperation &rop)
	{
		void* buffer = 0;

		const BufferElementGroup::ElementList& elmtList = rop.vertexData->bufferElementGroup->getElements();
		BufferElementGroup::ElementList::const_iterator elem, elemEnd;
		elemEnd = elmtList.end();

		for (elem = elmtList.begin(); elem != elemEnd; ++elem)
		{

			HardwareBuffer* vertexBuffer = vbl[elem->getSource()];
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

				/*if (mCurrentVertexProgram)
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
				else
				{
					// fixed function matching to units based on tex_coord_set
					for (i = 0; i < mDisabledTexUnitsFrom; i++)
					{
						// Only set this texture unit's texcoord pointer if it
						// is supposed to be using this element's index
						if (mTextureCoordIndex[i] == elem->getIndex() && i < mFixedFunctionTextureUnits)
						{
							if (multitexturing)
								glClientActiveTextureARB(GL_TEXTURE0 + i);
							glTexCoordPointer(
								VertexElement::getTypeCount(elem->getType()), 
								GLHardwareBufferManager::getGLType(elem->getType()),
								static_cast<GLsizei>(vertexBuffer->getVertexSize()), 
								pBufferData);
							glEnableClientState( GL_TEXTURE_COORD_ARRAY );
						}
					}
				}*/
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
				glDrawArrays(primType, 0, rop.vertexData->count);
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
};

class CGLShaderObject
{
protected:
	GLhandleARB m_hProgram;
	GLhandleARB m_hVertexShader;
	GLhandleARB m_hFragmentShader;
	std::map<std::string, GLint> m_mapParameters;

public:
	CGLShaderObject()
	{
		m_hProgram = NULL;
		m_hVertexShader = NULL;
		m_hFragmentShader = NULL;
	}
	~CGLShaderObject()
	{
		Cleanup();
	}

	bool IsValid()	{ return m_hProgram != NULL; }

	void Cleanup()
	{
		if(m_hFragmentShader)
		{
			glDeleteObjectARB(m_hFragmentShader);
			m_hFragmentShader = NULL;
		}
		if(m_hVertexShader)
		{
			glDeleteObjectARB(m_hVertexShader);
			m_hVertexShader = NULL;
		}
		if(m_hProgram)
		{
			glDeleteObjectARB(m_hProgram);
			m_hProgram = NULL;
		}
		m_mapParameters.clear();
	}

	bool Init(const char *pszVertexShader, const char *pszFragmentShader)
	{
		Cleanup();

		m_hProgram = glCreateProgramObjectARB();
		m_hVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		m_hFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

		char *psz;
		int nBytes, bSuccess;

		printf("Compiling GLSL vertex shader %s", pszVertexShader);
		std::ifstream ifVertexShader(pszVertexShader, std::ios::binary);
		if(!ifVertexShader)
		{
			printf("Failed to open %s", pszVertexShader);
			return false;
		}
		ifVertexShader.seekg(0, std::ios::end);
		nBytes = ifVertexShader.tellg();
		ifVertexShader.seekg(0, std::ios::beg);
		psz = new char[nBytes+1];
		ifVertexShader.read(psz, nBytes);
		psz[nBytes] = 0;
		ifVertexShader.close();
		glShaderSourceARB(m_hVertexShader, 1, (const char **)&psz, &nBytes);
		glCompileShaderARB(m_hVertexShader);
		glGetObjectParameterivARB(m_hVertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &bSuccess);
		delete psz;
		if(!bSuccess)
		{
			printf("Failed to compile vertex shader %s", pszVertexShader);
			return false;
		}

		printf("Compiling GLSL fragment shader %s", pszFragmentShader);
		std::ifstream ifFragmentShader(pszFragmentShader, std::ios::binary);
		if(!ifFragmentShader)
		{
			printf("Failed to open %s", pszFragmentShader);
			return false;
		}
		ifFragmentShader.seekg(0, std::ios::end);
		nBytes = ifFragmentShader.tellg();
		ifFragmentShader.seekg(0, std::ios::beg);
		psz = new char[nBytes];
		ifFragmentShader.read(psz, nBytes);
		ifFragmentShader.close();
		glShaderSourceARB(m_hFragmentShader, 1, (const char **)&psz, &nBytes);
		glCompileShaderARB(m_hFragmentShader);
		glGetObjectParameterivARB(m_hFragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &bSuccess);
		delete psz;
		if(!bSuccess)
		{
			printf("Failed to compile fragment shader %s", pszFragmentShader);
			return false;
		}

		glAttachObjectARB(m_hProgram, m_hVertexShader);
		glAttachObjectARB(m_hProgram, m_hFragmentShader);
		glLinkProgramARB(m_hProgram);

		glGetObjectParameterivARB(m_hProgram, GL_OBJECT_LINK_STATUS_ARB, &bSuccess);
		if(!bSuccess)
		{
			printf("Failed to link shaders %s and %s", pszVertexShader, pszFragmentShader);
			return false;
		}
		return true;
	}

	void Enable()
	{
		glUseProgramObjectARB(m_hProgram);
	}
	void Disable()
	{
		glUseProgramObjectARB(NULL);
	}

	GLint GetUniformParameterID(const char *pszParameter)
	{
		std::map<std::string, GLint>::iterator it = m_mapParameters.find(pszParameter);
		if(it == m_mapParameters.end())
		{
			GLint nLoc = glGetUniformLocationARB(m_hProgram, pszParameter);
			it = m_mapParameters.insert(std::pair<std::string, GLint>(pszParameter, nLoc)).first;
		}
		return it->second;
	}

	/*
	void BindTexture(const char *pszParameter, unsigned int nID)
	{
	GLint n = GetUniformParameterID(pszParameter);
	glBindTexture(GL_TEXTURE_2D, nID);
	glUniform1iARB(n, nID);
	}
	*/
	void SetUniformParameter1i(const char *pszParameter, int n1)
	{
		glUniform1iARB(GetUniformParameterID(pszParameter), n1);
	}
	void SetUniformParameter1f(const char *pszParameter, float p1)
	{
		glUniform1fARB(GetUniformParameterID(pszParameter), p1);
	}
	void SetUniformParameter3f(const char *pszParameter, float p1, float p2, float p3)
	{
		glUniform3fARB(GetUniformParameterID(pszParameter), p1, p2, p3);
	}
};

class gpuAtmosphericScattering
{
protected:
	int m_nPolygonMode;
	bool m_bHDR;

	Vector3f m_vLight;
	Vector3f m_vLightDirection;

	// Variables that can be tweaked with keypresses
	bool m_bShowTexture;
	int m_nSamples;
	float m_Kr, m_Kr4PI;
	float m_Km, m_Km4PI;
	float m_ESun;
	float m_g;

	float m_fInnerRadius;
	float m_fOuterRadius;
	float m_fScale;
	float m_fWavelength[3];
	float m_fWavelength4[3];
	float m_fRayleighScaleDepth;
	float m_fMieScaleDepth;

	CGLShaderObject m_shSkyFromSpace;
	CGLShaderObject m_shSkyFromAtmosphere;
	CGLShaderObject m_shGroundFromSpace;
	CGLShaderObject m_shGroundFromAtmosphere;

	// HDR variables, not used for now
	CGLShaderObject m_shHDR;
	float m_fExposure;
public:
	bool Start()
	{
		m_nPolygonMode = GL_FILL;
		m_vLight = Vector3f(1000, 1000, 1000);
		m_vLightDirection = m_vLight / m_vLight.magnitude();

		m_nSamples = 2;		// Number of sample rays to use in integral equation
		m_Kr = 0.0025f;		// Rayleigh scattering constant
		m_Kr4PI = m_Kr*4.0f*M_PI;
		m_Km = 0.0015f;		// Mie scattering constant
		m_Km4PI = m_Km*4.0f*M_PI;
		m_ESun = 15.0f;		// Sun brightness constant
		m_g = -0.95f;		// The Mie phase asymmetry factor

		m_fInnerRadius = 10.0f * 10.0;
		m_fOuterRadius = 10.25f * 10.0;
		m_fScale = 1 / (m_fOuterRadius - m_fInnerRadius);
		m_fScale *= 1000.0;

		m_fWavelength[0] = 0.650f;		// 650 nm for red
		m_fWavelength[1] = 0.570f;		// 570 nm for green
		m_fWavelength[2] = 0.475f;		// 475 nm for blue
		m_fWavelength4[0] = powf(m_fWavelength[0], 4.0f);
		m_fWavelength4[1] = powf(m_fWavelength[1], 4.0f);
		m_fWavelength4[2] = powf(m_fWavelength[2], 4.0f);

		m_fRayleighScaleDepth = 0.25f;
		m_fMieScaleDepth = 0.1f;

		m_shSkyFromSpace.Init("shaders/SkyFromSpaceVert.glsl", "shaders/SkyFromSpaceFrag.glsl");
		m_shSkyFromAtmosphere.Init("shaders/SkyFromAtmosphereVert.glsl", "shaders/SkyFromAtmosphereFrag.glsl");
		m_shGroundFromSpace.Init("shaders/GroundFromSpaceVert.glsl", "shaders/GroundFromSpaceFrag.glsl");
		m_shGroundFromAtmosphere.Init("shaders/GroundFromAtmosphereVert.glsl", "shaders/GroundFromAtmosphereFrag.glsl");

		m_fExposure = 2.0f;
		m_shHDR.Init("shaders/HDRVert.glsl", "shaders/HDRFrag.glsl");
		m_bHDR = false;
		return true;
	}

	void Update(const Phoenix::Camera &cam)
	{
		glClearColor(0,0,0,0);
		// Cheap collision detection/response
		Vector3f vCamera = cam.getPosition();
		/*
		if(vCamera.magnitude() < m_fInnerRadius + 0.01f)
		{
			Vector3f N = vCamera / vCamera.Magnitude();
			Vector3f I = CCameraTask::GetPtr()->GetVelocity();
			float fSpeed = I.magnitude();
			I /= fSpeed;
			Vector3f R = N * (2.0*(-I | N)) + I;
			CCameraTask::GetPtr()->SetVelocity(R * fSpeed);

			vCamera = N * (m_fInnerRadius + 0.01f);
			CCameraTask::GetPtr()->SetPosition(vCamera);
		}
		*/
		//Vector3f vUnitCamera = vCamera / vCamera.magnitude();

		glPolygonMode(GL_FRONT, m_nPolygonMode);

		// Draw the groud sphere
		CGLShaderObject *pGroundShader;
		if(vCamera.magnitude() >= m_fOuterRadius)
			pGroundShader = &m_shGroundFromSpace;
		else
			pGroundShader = &m_shGroundFromAtmosphere;
		pGroundShader->Enable();
		pGroundShader->SetUniformParameter3f("v3CameraPos", vCamera.x, vCamera.y, vCamera.z);
		pGroundShader->SetUniformParameter3f("v3LightPos", m_vLightDirection.x, m_vLightDirection.y, m_vLightDirection.z);
		pGroundShader->SetUniformParameter3f("v3InvWavelength", 1/m_fWavelength4[0], 1/m_fWavelength4[1], 1/m_fWavelength4[2]);
		pGroundShader->SetUniformParameter1f("fCameraHeight", vCamera.magnitude());
		pGroundShader->SetUniformParameter1f("fCameraHeight2", (vCamera.magnitude() * vCamera.magnitude()));
		pGroundShader->SetUniformParameter1f("fInnerRadius", m_fInnerRadius);
		pGroundShader->SetUniformParameter1f("fInnerRadius2", m_fInnerRadius*m_fInnerRadius);
		pGroundShader->SetUniformParameter1f("fOuterRadius", m_fOuterRadius);
		pGroundShader->SetUniformParameter1f("fOuterRadius2", m_fOuterRadius*m_fOuterRadius);
		pGroundShader->SetUniformParameter1f("fKrESun", m_Kr*m_ESun);
		pGroundShader->SetUniformParameter1f("fKmESun", m_Km*m_ESun);
		pGroundShader->SetUniformParameter1f("fKr4PI", m_Kr4PI);
		pGroundShader->SetUniformParameter1f("fKm4PI", m_Km4PI);
		pGroundShader->SetUniformParameter1f("fScale", 1.0f / (m_fOuterRadius - m_fInnerRadius));
		pGroundShader->SetUniformParameter1f("fScaleDepth", m_fRayleighScaleDepth);
		pGroundShader->SetUniformParameter1f("fScaleOverScaleDepth", (1.0f / (m_fOuterRadius - m_fInnerRadius)) / m_fRayleighScaleDepth);
		pGroundShader->SetUniformParameter1f("g", m_g);
		pGroundShader->SetUniformParameter1f("g2", m_g*m_g);
		pGroundShader->SetUniformParameter1i("nSamples", m_nSamples);
		pGroundShader->SetUniformParameter1f("fSamples", m_nSamples);
		pGroundShader->SetUniformParameter1i("s2Test", 0);
		GLUquadricObj *pSphere = gluNewQuadric();
		//m_tEarth.Enable();
		gluSphere(pSphere, m_fInnerRadius, 100, 50);
		//m_tEarth.Disable();
		gluDeleteQuadric(pSphere);
		pGroundShader->Disable();

		// Draw the sky sphere
		CGLShaderObject *pSkyShader;
		if(vCamera.magnitude() >= m_fOuterRadius)
			pSkyShader = &m_shSkyFromSpace;
		else
			pSkyShader = &m_shSkyFromAtmosphere;
		pSkyShader->Enable();
		pSkyShader->SetUniformParameter3f("v3CameraPos", vCamera.x, vCamera.y, vCamera.z);
		pSkyShader->SetUniformParameter3f("v3LightPos", m_vLightDirection.x, m_vLightDirection.y, m_vLightDirection.z);
		pSkyShader->SetUniformParameter3f("v3InvWavelength", 1/m_fWavelength4[0], 1/m_fWavelength4[1], 1/m_fWavelength4[2]);
		pSkyShader->SetUniformParameter1f("fCameraHeight", vCamera.magnitude());
		pSkyShader->SetUniformParameter1f("fCameraHeight2", (vCamera.magnitude() * vCamera.magnitude()));
		pSkyShader->SetUniformParameter1f("fInnerRadius", m_fInnerRadius);
		pSkyShader->SetUniformParameter1f("fInnerRadius2", m_fInnerRadius*m_fInnerRadius);
		pSkyShader->SetUniformParameter1f("fOuterRadius", m_fOuterRadius);
		pSkyShader->SetUniformParameter1f("fOuterRadius2", m_fOuterRadius*m_fOuterRadius);
		pSkyShader->SetUniformParameter1f("fKrESun", m_Kr*m_ESun);
		pSkyShader->SetUniformParameter1f("fKmESun", m_Km*m_ESun);
		pSkyShader->SetUniformParameter1f("fKr4PI", m_Kr4PI);
		pSkyShader->SetUniformParameter1f("fKm4PI", m_Km4PI);
		pSkyShader->SetUniformParameter1f("fScale", 1.0f / (m_fOuterRadius - m_fInnerRadius));
		pSkyShader->SetUniformParameter1f("fScaleDepth", m_fRayleighScaleDepth);
		pSkyShader->SetUniformParameter1f("fScaleOverScaleDepth", (1.0f / (m_fOuterRadius - m_fInnerRadius)) / m_fRayleighScaleDepth);
		pSkyShader->SetUniformParameter1f("g", m_g);
		pSkyShader->SetUniformParameter1f("g2", m_g*m_g);
		pSkyShader->SetUniformParameter1i("nSamples", m_nSamples);
		pSkyShader->SetUniformParameter1f("fSamples", m_nSamples);
		//m_tOpticalDepth.Enable();
		pSkyShader->SetUniformParameter1f("tex", 0);
		glFrontFace(GL_CW);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		pSphere = gluNewQuadric();
		gluSphere(pSphere, m_fOuterRadius, 100, 100);
		gluDeleteQuadric(pSphere);
		glDisable(GL_BLEND);
		glFrontFace(GL_CCW);
		//m_tOpticalDepth.Disable();
		pSkyShader->Disable();

		glPolygonMode(GL_FRONT, GL_FILL);
	}

	void Stop()
	{
		m_shSkyFromSpace.Cleanup();
		m_shSkyFromAtmosphere.Cleanup();
		m_shGroundFromSpace.Cleanup();
		m_shGroundFromAtmosphere.Cleanup();
	}
};


void setCamera(const Vector3f &pos,
			   const Vector3f &target)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos.x, pos.y, pos.z,
			  target.x, target.y, target.z,
			  0, 1, 0);
}

GLfloat vertices[] = {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,
                      1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,
                      1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,
                      -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,
                      -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,
                      1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};

GLubyte indices[] = {0,1,2,3,
                     4,5,6,7,
                     8,9,10,11,
                     12,13,14,15,
                     16,17,18,19,
                     20,21,22,23};

class DemoWindow
	: public Window
{
	Shay::ShayWorld s;
	Phoenix::Camera c;
	ObjectFactory<GameEntity*, std::string, GameEntity*(*)()> EntityFactory;
	HardwareBuffer *vertexBuffer;
	HardwareBuffer *indexBuffer;
	gpuAtmosphericScattering atm;
	bool mSwitchWorld;
	RenderOperation rop;
	Renderer renderer;
	Model model;
	SwmReader swmR;
	ResourcePtr texture;
public:
	DemoWindow(const std::string& name,
		const Resolution_us& res,
		bool fullscreen)
		: Window(NULL, name, res, fullscreen),
		mSwitchWorld(false)
	{
	}

	void drawFlatPatch(float rows, float columns, float size)
	{
		const float m = 1.0f/columns;
		const float n = 1.0f/rows;
		int i, j;

		for (i = 0; i < columns; i++)
		{
			glBegin(GL_QUAD_STRIP);
			for (j = 0; j <= rows; j++)
			{
				glTexCoord2f(i, j);
				glVertex2f(i * m * size, j * n * size);
				glTexCoord2f((i+1), j);
				glVertex2f((i + 1) * m * size, j * n * size);
			}
			glTexCoord2f(i, 0);
			glVertex2f(i * m * size, 0);
			glTexCoord2f((i+1), 0);
			glVertex2f((i + 1) * size * m, 0);
			glEnd();
		}
	}

	void onDisplay()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		c.doLook();
		
		if (!mSwitchWorld)
		{
			glClearColor(97.0/255.0, 140.0/255.0, 185.0/255.0, 1.0);
			glPushMatrix();
			//s.DrawBackdrop();
			glPopMatrix();
		}
		else
		{
			atm.Update(c);
		}
		renderer.render(rop);

		//glEnable(GL_TEXTURE_2D);
		static_cast<Texture*>(texture.get())->bind(0);
		glPushMatrix();
		glTranslatef(70.0, 35.0, 0.0);
		drawFlatPatch(16.0f,16.0f,256.0f);
		glPopMatrix();
		static_cast<Texture*>(texture.get())->unbind();

		/*
		texture.bind(0);
		glColor4f(1, 1, 1, 1);
		float backRect[4] = { 50 - 24, 50 - 128, 196, 180 };
		glBegin (GL_QUADS);
		glTexCoord2f(-1.0, -1.0); glVertex2f(-1.0 * backRect[2], -1.0 * backRect[3]);
		glTexCoord2f( 1.0, -1.0); glVertex2f( 1.0 * backRect[2], -1.0 * backRect[3]);
		glTexCoord2f( 1.0,  1.0); glVertex2f( 1.0 * backRect[2],  1.0 * backRect[3]);
		glTexCoord2f(-1.0,  1.0); glVertex2f(-1.0 * backRect[2],  1.0 * backRect[3]);
		glEnd();
		texture.unbind();
		*/

		glutSwapBuffers();
	}

	void onIdle()
	{
		glutPostRedisplay();
	}

	void onReshape(int w, int h)
	{
		glViewport(0,0,static_cast<GLsizei>(w), static_cast<GLsizei>(h));
		float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, aspectRatio, 1.0, 100000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void onKeyboard(unsigned char key, int x, int y)
	{
		float factor = 1.0f;
		if (!mSwitchWorld)
			factor = 100.0f;
		else
			factor = 1.0f;

		switch (key)
		{

		case 'w':	//Forward
			c.moveRelative(Vector3f(0,0,factor));
			break;
		case 's':	//Backward
			c.moveRelative(Vector3f(0,0,-factor));
			break;
		case 'a':	//Strafe left
			c.moveRelative(Vector3f(factor,0,0));
			break;
		case 'd':	//Strafe right
			c.moveRelative(Vector3f(-factor,0,0));
			break;

		case 'i':	//Rotate up
			c.pitch(static_cast<Units::Radians>(10 * Units::degrees));
			break;
		case 'k':	//Rotate down
			c.pitch(static_cast<Units::Radians>(-10 * Units::degrees));
			break;
		case 'j':	//Rotate left
			c.rotate(Vector3f::Y, static_cast<Units::Radians>(10 * Units::degrees));
			break;
		case 'l':	//Rotate right
			c.rotate(Vector3f::Y, static_cast<Units::Radians>(-10 * Units::degrees));
			break;

		case 'p':	//Print current coordinates
			std::cout << c.getPosition() << std::endl;
			break;

		case 'f':	//Switch worlds
			if (!mSwitchWorld)
			{
				//c.move(Vector3f(0.0, 0.0, -25.0));
				c.setPosition(Vector3f(0.0, 0.0, -150.0));
				c.setDirection(Vector3f(0.0, 0.0, 0.0));
			}
			else
			{
				c.setPosition(Vector3f(4000.0, 10450.0, 20620.0));
				c.setDirection(Vector3f(3999.0, 10450.0, 20620.0));
			}
			mSwitchWorld = !mSwitchWorld;
			break;
		};

		glutPostRedisplay();
	}

protected:
	void initializeImpl()
	{
		swmR.readFile("raider.swm", model);
		const std::vector<GLfloat> &mdlVertices = model.getVerticeVec();
		vertexBuffer = new HardwareBuffer(sizeof(&mdlVertices[0]) * mdlVertices.size(), HardwareBuffer::STATIC_DRAW, HardwareBuffer::VERTEX);
		vertexBuffer->upload(&mdlVertices[0]);
		const std::vector<FaceGroup>& fg = model.getFaceGroupVec();
		std::vector<GLuint> mdlIndices;
		for(std::vector<FaceGroup>::const_iterator groupIter = fg.begin(); groupIter != fg.end(); ++groupIter)
		{
			for(std::vector<FaceCollection>::const_iterator faceIter = (*groupIter).faces.begin(); faceIter != (*groupIter).faces.end(); faceIter++ )
			{
				mdlIndices.insert(mdlIndices.end(), (*faceIter).v.begin(), (*faceIter).v.end()); 
			}
		}
		indexBuffer = new HardwareBuffer(sizeof(&mdlIndices[0]) * mdlIndices.size(), HardwareBuffer::STATIC_DRAW, HardwareBuffer::INDEX);
		indexBuffer->upload(&mdlIndices[0]);

		vbl[0] = vertexBuffer;

		rop.indexData = new IndexData();
		rop.indexData->indexBuffer = indexBuffer;
		rop.indexData->count = mdlIndices.size();
		rop.indexData->start = 0;
		rop.indexed = true;

		rop.vertexData = new VertexData();
		rop.vertexData->count = mdlVertices.size();
		rop.vertexData->bufferElementGroup->addElement(0,0,POSITION,FLOAT3);

		rop.primitiveType = RenderOperation::TRIANGLES;

		glShadeModel(GL_SMOOTH);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		//s.myinit();
		//c.move(Vector3f(0.0, 10450.0, 0.0));
		//c.setPosition(Vector3f(4000.0, 10450.0, 20620.0));
		//c.setDirection(Vector3f(3999.0, 10450.0, 20620.0));
		//c.rotate(Vector3f::Y, static_cast<Math::Units::Radians>(270 * Math::Units::degrees));

		texture = TexturePtr(new Texture(NULL, "floor_texture", 0, TEXTURE_2D));
		texture->setFilePath("floor_color_map.tga");
		texture->prepare();
		texture->load();
		atm.Start();
	}

	void destroyImpl()
	{
		delete vertexBuffer;
		delete indexBuffer;
		atm.Stop();
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