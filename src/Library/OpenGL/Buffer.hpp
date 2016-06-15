#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <GL/glew.h>
#include <vector>
#include <utility>

enum class Usage
{
	Static = GL_STATIC_DRAW,
	Dynamic = GL_DYNAMIC_DRAW
};

class Buffer
{
public:
	Buffer( GLenum type );
	virtual ~Buffer();

	Buffer( Buffer&& buffer );
	Buffer& operator=( Buffer&& buffer );

	void bind() const;
	void unbind() const;

	template< typename T >
	void setData( const std::vector< T >& data, Usage usage );

private:
	GLuint m_handle;
	GLenum m_type;
};

template< typename T >
void Buffer::setData( const std::vector< T >& data, Usage usage )
{
	bind();
	glBufferData( m_type, sizeof( T ) * data.size(), &data[ 0 ], static_cast< GLenum >( usage ) );
	unbind();
}

class VertexBuffer : public Buffer
{
public:
	VertexBuffer();
};

class IndexBuffer : public Buffer
{
public:
	IndexBuffer();
};

class InstanceBuffer : public Buffer
{
public:
	InstanceBuffer();
};
#endif