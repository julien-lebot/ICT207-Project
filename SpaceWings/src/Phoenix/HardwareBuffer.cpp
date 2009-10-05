#include <Phoenix/HardwareBuffer.hpp>

using namespace Phoenix;

GLenum getGLUsage(const HardwareBuffer::Usage usage)
{
	switch (usage)
	{
	case HardwareBuffer::STREAM_DRAW:
		return GL_STREAM_DRAW;
		break;
	case HardwareBuffer::STREAM_READ:
		return GL_STREAM_READ;
		break;
	case HardwareBuffer::STREAM_COPY:
		return GL_STREAM_COPY;
		break;
	case HardwareBuffer::STATIC_DRAW:
		return GL_STATIC_DRAW;
		break;
	case HardwareBuffer::STATIC_READ:
		return GL_STATIC_READ;
		break;
	case HardwareBuffer::STATIC_COPY:
		return GL_STATIC_COPY;
		break;
	case HardwareBuffer::DYNAMIC_DRAW:
		return GL_DYNAMIC_DRAW;
		break;
	case HardwareBuffer::DYNAMIC_READ:
		return GL_DYNAMIC_READ;
		break;
	case HardwareBuffer::DYNAMIC_COPY:
		return GL_DYNAMIC_COPY;
		break;
	default:
		return GL_STATIC_DRAW;
	};
}

GLenum getGLAccess(const HardwareBuffer::Access access)
{
	switch(access)
	{
	case HardwareBuffer::READ_ONLY:
		return GL_READ_ONLY;
		break;
	case HardwareBuffer::READ_WRITE:
		return GL_READ_WRITE;
		break;
	case HardwareBuffer::WRITE_ONLY:
		return GL_WRITE_ONLY;
		break;
	default:
		return GL_READ_ONLY;
		break;
	};
}

GLenum getGLTarget(const HardwareBuffer::Target target)
{
	switch (target)
	{
	case HardwareBuffer::VERTEX:
		return GL_ARRAY_BUFFER;
		break;
	case HardwareBuffer::INDEX:
		return GL_ELEMENT_ARRAY_BUFFER;
		break;
	default:
		return GL_ARRAY_BUFFER;
		break;
	};
}

HardwareBuffer::HardwareBuffer(const std::size_t size,
							   const Usage usage,
							   const Target target)
: mSize(size),
  mUsage(usage),
  mTarget(target)
{
	glGenBuffers(1, &mID);
	glBindBuffer(getGLTarget(mTarget), mID);
	glBufferData(getGLTarget(mTarget), mSize, NULL, getGLUsage(mUsage));
}

HardwareBuffer::~HardwareBuffer()
{
	glDeleteBuffers(1, &mID);
}

void HardwareBuffer::bind()
{
	glBindBuffer(getGLTarget(mTarget), mID);
}

void HardwareBuffer::unbind()
{
	glBindBuffer(getGLTarget(mTarget), 0);
}

GLuint HardwareBuffer::getID()
{
	return mID;
}

void HardwareBuffer::upload(const void* data)
{
	glBindBuffer(getGLTarget(mTarget), mID);
	glBufferData(getGLTarget(mTarget), mSize, data, getGLUsage(mUsage));
}

void HardwareBuffer::upload(int offset, std::size_t size, const void* data)
{
	glBindBuffer(getGLTarget(mTarget), mID);
	glBufferSubData(getGLTarget(mTarget), offset, size, data);
}

void * HardwareBuffer::map(const Access access)
{
	return glMapBuffer(getGLTarget(mTarget), getGLAccess(access));
}

void HardwareBuffer::unmap()
{
	glUnmapBuffer(getGLTarget(mTarget));
}
