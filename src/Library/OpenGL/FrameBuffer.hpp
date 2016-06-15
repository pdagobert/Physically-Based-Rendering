#ifndef __FRAME_BUFFER_HPP__
#define __FRAME_BUFFER_HPP__

#include "RenderBuffer.hpp"
#include <GL/glew.h>

class Texture2D;
class CubeMap;

enum class Attachment
{
	Color0 = GL_COLOR_ATTACHMENT0,
	Depth = GL_DEPTH_ATTACHMENT
};

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	FrameBuffer( FrameBuffer&& frameBuffer );
	FrameBuffer& operator=( FrameBuffer&& frameBuffer );

	FrameBuffer( const FrameBuffer& ) = delete;
	FrameBuffer& operator=( const FrameBuffer& ) = delete;

	void attach( Attachment attachment, const Texture2D& texture );
	void attach( Attachment attachment, const CubeMap& cubemap, int face );
	void attach( Attachment attachment, const RenderBuffer& renderBuffer );

	void bind() const;
	void unbind() const;

private:
	GLuint m_handle;
};
#endif