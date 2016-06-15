#ifndef __VERTEX_BUFFER_DATA_HPP__
#define __VERTEX_BUFFER_DATA_HPP__

#include "VertexLayout.hpp"
#include <vector>
#include <glm/glm.hpp>

class VertexBufferData
{
public:
	VertexBufferData( VertexLayout layout, int size );

	void bind() const;

	void set( int index, int attribute, const glm::vec4& value );
	void set( int index, int attribute, const glm::vec3& value );
	void set( int index, int attribute, const glm::vec2& value );
	void set( int index, int attribute, float value );

	void add( int index, int attribute, const glm::vec3& value );

	float get( int index, int attribute, int offset ) const;

	unsigned int getVerticesNumber() const;
	const std::vector< float >& getData() const;

private:
	VertexLayout m_layout;
	std::vector< float > m_vertices;
};
#endif