#ifndef __RENDER_BUFFER_HPP__
#define __RENDER_BUFFER_HPP__

#include <GL/glew.h>

enum class Storage
{
	Depth24 = GL_DEPTH_COMPONENT24,
	Depth32 = GL_DEPTH_COMPONENT32F
};

class RenderBuffer
{
public:
	RenderBuffer();
	RenderBuffer( int width, int height, Storage storage );
	~RenderBuffer();

	RenderBuffer( RenderBuffer&& renderBuffer );
	RenderBuffer& operator=( RenderBuffer&& renderBuffer );

	GLuint getHandle() const;

private:
	GLuint m_handle;
};
#endif