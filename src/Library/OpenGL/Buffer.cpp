#include "Buffer.hpp"

Buffer::Buffer( GLenum type ) :
	m_type( type )
{
	glGenBuffers( 1, &m_handle );
}

Buffer::Buffer( Buffer&& model ) :
	m_handle( model.m_handle ),
	m_type( model.m_type )
{
	model.m_handle = 0;
}

Buffer& Buffer::operator=( Buffer&& buffer )
{
	m_handle = buffer.m_handle;
	m_type = buffer.m_type;
	
	buffer.m_handle = 0;
	return *this;
}

Buffer::~Buffer()
{
	if( m_handle != 0 )
		glDeleteBuffers( 1, &m_handle );
}

void Buffer::bind() const
{
	glBindBuffer( m_type, m_handle );
}

void Buffer::unbind() const
{
	glBindBuffer( m_type, 0 );
}

VertexBuffer::VertexBuffer() :
	Buffer( GL_ARRAY_BUFFER )
{

}

IndexBuffer::IndexBuffer() :
	Buffer( GL_ELEMENT_ARRAY_BUFFER )
{
	
}

InstanceBuffer::InstanceBuffer() :
	Buffer( GL_ARRAY_BUFFER )
{
	
}