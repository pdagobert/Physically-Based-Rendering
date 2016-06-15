#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

#include "VertexBufferData.hpp"
#include "IndexBufferData.hpp"
#include "VertexLayout.hpp"
#include <functional>

struct Geometry
{
	Geometry( VertexBufferData vertices, IndexBufferData indices ) :
		vertices( vertices ),
		indices( indices )
	{

	}

	VertexBufferData vertices;
	IndexBufferData indices;
};

Geometry createUVSphere( float radius, int precision, VertexLayout layout );

Geometry createBox( const glm::vec3& midSize, VertexLayout layout );

float defaultHeightFunc( int, int );

Geometry createSubdivisedPlane( int width, int height, float size, VertexLayout layout, std::function< float( int, int ) > heightFunc = &defaultHeightFunc );
Geometry createUnitQuad( VertexLayout layout );

void computeSmoothNormals( VertexBufferData& vertices, const IndexBufferData& indices );
#endif