#include "VertexBufferData.hpp"

VertexBufferData::VertexBufferData( VertexLayout layout, int size ) :
	m_layout( layout ),
	m_vertices( layout.getSize() * size, 0.0f )
{

}

void VertexBufferData::bind() const
{
	int i = 0;

	for( auto it = m_layout.getAttributes().begin(); it != m_layout.getAttributes().end(); it++ )
	{
		const Attribute attrib = it->second;

		glEnableVertexAttribArray( i );
		glVertexAttribPointer( i, attrib.size, GL_FLOAT, GL_FALSE, m_layout.getSize() * 4, reinterpret_cast< void* >( attrib.offset * 4 ) );

		i++;
	}
}

void VertexBufferData::set( int index, int attribute, const glm::vec4& value )
{
	int vertexSize = m_layout.getSize();
	int offset = m_layout.getOffset( attribute );

	for( int i = 0; i < 4; i++ )
		m_vertices[ index * vertexSize + offset + i ] = value[ i ];
}

void VertexBufferData::set( int index, int attribute, const glm::vec3& value )
{
	int vertexSize = m_layout.getSize();
	int offset = m_layout.getOffset( attribute );

	for( int i = 0; i < 3; i++ )
		m_vertices[ index * vertexSize + offset + i ] = value[ i ];
}

void VertexBufferData::set( int index, int attribute, const glm::vec2& value )
{
	int vertexSize = m_layout.getSize();
	int offset = m_layout.getOffset( attribute );

	for( int i = 0; i < 2; i++ )
		m_vertices[ index * vertexSize + offset + i ] = value[ i ];
}

void VertexBufferData::set( int index, int attribute, float value )
{
	int vertexSize = m_layout.getSize();
	int offset = m_layout.getOffset( attribute );

	m_vertices[ index * vertexSize + offset ] = value;
}

void VertexBufferData::add( int index, int attribute, const glm::vec3& value )
{
	int vertexSize = m_layout.getSize();
	int offset = m_layout.getOffset( attribute );

	for( int i = 0; i < 3; i++ )
		m_vertices[ index * vertexSize + offset + i ] += value[ i ];
}

float VertexBufferData::get( int index, int attribute, int offset ) const
{
	int vertexSize = m_layout.getSize();
	int attributeOffset = m_layout.getOffset( attribute );

	return m_vertices[ index * vertexSize + attributeOffset + offset ];
}

unsigned int VertexBufferData::getVerticesNumber() const
{
	return m_vertices.size() / m_layout.getSize();
}

const std::vector< float >& VertexBufferData::getData() const
{
	return m_vertices;
}