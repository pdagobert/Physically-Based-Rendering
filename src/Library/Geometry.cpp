#include "Geometry.hpp"
#include <glm/gtc/constants.hpp>

Geometry createUVSphere( float radius, int precision, VertexLayout layout )
{
	VertexBufferData vertices( layout, precision * precision );

	float inv = 1.0f / ( precision - 1.0f );

	int index = 0;

	for( int i = 0; i < precision; i++ )
	{
		for( int j = 0; j < precision; j++ )
		{
			float x = glm::cos( 2.0f * glm::pi< float >() * j * inv ) * glm::sin( glm::pi< float >() * i * inv );
			float y = glm::sin( -glm::pi< float >() / 2.0f + glm::pi< float >() * i * inv );
			float z = glm::sin( 2.0f * glm::pi< float >() * j * inv ) * glm::sin( glm::pi< float >() * i * inv );

			glm::vec3 normal = glm::vec3( x, y, z );
			vertices.set( index, VertexAttribute::Position, normal * radius );

			if( layout.has( VertexAttribute::TexCoords ) )
				vertices.set( index, VertexAttribute::TexCoords, glm::vec2( j * inv, 1.0 - i * inv ) );

			if( layout.has( VertexAttribute::Normal ) )
				vertices.set( index, VertexAttribute::Normal, normal );

			index++;
		}
	}

	IndexBufferData indices( ( precision - 1 ) * ( precision - 1 ) * 6 );

	index = 0;

	for( int i = 0; i < precision - 1; i++ )
	{
		for( int j = 0; j < precision - 1; j++ )
		{
			indices.set( index++, i * precision + j );
			indices.set( index++, i * precision + j + 1 );
			indices.set( index++, ( i + 1 ) * precision + j );
			indices.set( index++, ( i + 1 ) * precision + j );
			indices.set( index++, i * precision + j + 1 );
			indices.set( index++, ( i + 1 ) * precision + j + 1 );
		}
	}

	return Geometry( vertices, indices );
}

Geometry createBox( const glm::vec3& midSize, VertexLayout layout )
{
	VertexBufferData vertices( layout, 36 );

	vertices.set( 0, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, midSize.z ) );
	vertices.set( 1, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, midSize.z ) );
	vertices.set( 2, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, midSize.z ) );
	vertices.set( 3, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, midSize.z ) );
	vertices.set( 4, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, midSize.z ) );
	vertices.set( 5, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, midSize.z ) );

	vertices.set( 6, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, midSize.z ) );
	vertices.set( 7, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, -midSize.z ) );
	vertices.set( 8, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, midSize.z ) );
	vertices.set( 9, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, midSize.z ) );
	vertices.set( 10, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, -midSize.z ) );
	vertices.set( 11, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, -midSize.z ) );

	vertices.set( 12, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, -midSize.z ) );
	vertices.set( 13, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, -midSize.z ) );
	vertices.set( 14, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, -midSize.z ) );
	vertices.set( 15, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, -midSize.z ) );
	vertices.set( 16, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, -midSize.z ) );
	vertices.set( 17, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, -midSize.z ) );

	vertices.set( 18, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, -midSize.z ) );
	vertices.set( 19, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, midSize.z ) );
	vertices.set( 20, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, -midSize.z ) );
	vertices.set( 21, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, -midSize.z ) );
	vertices.set( 22, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, midSize.z ) );
	vertices.set( 23, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, midSize.z ) );

	vertices.set( 24, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, midSize.z ) );
	vertices.set( 25, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, midSize.z ) );
	vertices.set( 26, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, -midSize.z ) );
	vertices.set( 27, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, -midSize.z ) );
	vertices.set( 28, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, midSize.z ) );
	vertices.set( 29, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, -midSize.z ) );

	vertices.set( 30, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, midSize.z ) );
	vertices.set( 31, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, midSize.z ) );
	vertices.set( 32, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, -midSize.z ) );
	vertices.set( 33, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, -midSize.z ) );
	vertices.set( 34, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, midSize.z ) );
	vertices.set( 35, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, -midSize.z ) );

	if( layout.has( VertexAttribute::TexCoords ) )
	{
		for( int i = 0; i < 6; i++ )
		{
			vertices.set( i * 6 + 0, VertexAttribute::TexCoords, glm::vec2( 0.0f, 0.0f ) );
			vertices.set( i * 6 + 1, VertexAttribute::TexCoords, glm::vec2( 1.0f, 0.0f ) );
			vertices.set( i * 6 + 2, VertexAttribute::TexCoords, glm::vec2( 0.0f, 1.0f ) );
			vertices.set( i * 6 + 3, VertexAttribute::TexCoords, glm::vec2( 0.0f, 1.0f ) );
			vertices.set( i * 6 + 4, VertexAttribute::TexCoords, glm::vec2( 1.0f, 0.0f ) );
			vertices.set( i * 6 + 5, VertexAttribute::TexCoords, glm::vec2( 1.0f, 1.0f ) );	
		}
	}

	IndexBufferData indices( 36 );

	for( int i = 0; i < 36; i++ )
		indices.set( i, i );

	if( layout.has( VertexAttribute::Normal ) )
		computeSmoothNormals( vertices, indices );

	if( layout.has( VertexAttribute::Tangent ) )
	{
		vertices.set( 0, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );	
		vertices.set( 1, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 2, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 3, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 4, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 5, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );

		vertices.set( 6, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, -1.0f ) );
		vertices.set( 7, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, -1.0f ) );
		vertices.set( 8, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, -1.0f ) );
		vertices.set( 9, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, -1.0f ) );
		vertices.set( 10, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, -1.0f ) );
		vertices.set( 11, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, -1.0f ) );

		vertices.set( 12, VertexAttribute::Tangent, glm::vec3( -1.0f, 0.0f, 0.0f ) );
		vertices.set( 13, VertexAttribute::Tangent, glm::vec3( -1.0f, 0.0f, 0.0f ) );
		vertices.set( 14, VertexAttribute::Tangent, glm::vec3( -1.0f, 0.0f, 0.0f ) );
		vertices.set( 15, VertexAttribute::Tangent, glm::vec3( -1.0f, 0.0f, 0.0f ) );
		vertices.set( 16, VertexAttribute::Tangent, glm::vec3( -1.0f, 0.0f, 0.0f ) );
		vertices.set( 17, VertexAttribute::Tangent, glm::vec3( -1.0f, 0.0f, 0.0f ) );

		vertices.set( 18, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, 1.0f ) );
		vertices.set( 19, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, 1.0f ) );
		vertices.set( 20, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, 1.0f ) );
		vertices.set( 21, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, 1.0f ) );
		vertices.set( 22, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, 1.0f ) );
		vertices.set( 23, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, 1.0f ) );

		vertices.set( 24, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 25, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 26, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 27, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 28, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 29, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );

		vertices.set( 30, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 31, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 32, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 33, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 34, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
		vertices.set( 35, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );

	}

	return Geometry( vertices, indices );
}

Geometry createSubdivisedPlane( int width, int height, float size, VertexLayout layout, std::function< float( int, int ) > heightFunc )
{
	int vWidth = width + 1;
	int vHeight = height + 1;

	VertexBufferData vertices( layout, vWidth * vHeight );

	for( int i = 0; i < vHeight; i++ )
	{
		for( int j = 0; j < vWidth; j++ )
		{
			int vertexIndex = i * vWidth + j;
			glm::vec3 position( j * size, heightFunc( j, i ), -i * size );

			vertices.set( vertexIndex, VertexAttribute::Position, position );
			
			if( layout.has( VertexAttribute::TexCoords ) )
				vertices.set( vertexIndex, VertexAttribute::TexCoords, glm::vec2( static_cast< float >( j ) / width, static_cast< float >( i ) / height ) );
		}
	}

	IndexBufferData indices( width * height * 6 );
	int index = 0;

	for( int i = 0; i < height; i++ )
	{
		for( int j = 0; j < width; j++ )
		{
			int firstIndex = i * vWidth + j;

			indices.set( index++, firstIndex );
			indices.set( index++, firstIndex + 1 );
			indices.set( index++, firstIndex + vWidth );
			indices.set( index++, firstIndex + vWidth );
			indices.set( index++, firstIndex + 1 );
			indices.set( index++, firstIndex + vWidth + 1 );
		}
	}

	if( layout.has( VertexAttribute::Normal ) )
		computeSmoothNormals( vertices, indices );

	return Geometry( vertices, indices );
}

Geometry createUnitQuad( VertexLayout layout )
{
	VertexBufferData vertices( layout, 4 );
	vertices.set( 0, VertexAttribute::Position, glm::vec3( -1.0f, -1.0f, 0.0f ) );
	vertices.set( 1, VertexAttribute::Position, glm::vec3( 1.0f, -1.0f, 0.0f ) );
	vertices.set( 2, VertexAttribute::Position, glm::vec3( -1.0f, 1.0f, 0.0f ) );
	vertices.set( 3, VertexAttribute::Position, glm::vec3( 1.0f, 1.0f, 0.0f ) );

	if( layout.has( VertexAttribute::TexCoords ) )
	{
		vertices.set( 0, VertexAttribute::TexCoords, glm::vec2( 0.0f, 0.0f ) );
		vertices.set( 1, VertexAttribute::TexCoords, glm::vec2( 1.0f, 0.0f ) );
		vertices.set( 2, VertexAttribute::TexCoords, glm::vec2( 0.0f, 1.0f ) );
		vertices.set( 3, VertexAttribute::TexCoords, glm::vec2( 1.0f, 1.0f ) );
	}

	IndexBufferData indices( 6 );
	indices.set( 0, 0 );
	indices.set( 1, 1 );
	indices.set( 2, 2 );
	indices.set( 3, 2 );
	indices.set( 4, 1 );
	indices.set( 5, 3 );

	return Geometry( vertices, indices );
}

void computeSmoothNormals( VertexBufferData& vertices, const IndexBufferData& indices )
{
	for( unsigned int i = 0; i < indices.getSize(); i += 3 )
	{
		glm::vec3 v0;
		glm::vec3 v1;
		glm::vec3 v2;

		for( int k = 0; k < 3; k++ )
			v0[ k ] = vertices.get( indices.get( i ), VertexAttribute::Position, k );

		for( int k = 0; k < 3; k++ )
			v1[ k ] = vertices.get( indices.get( i + 1 ), VertexAttribute::Position, k );

		for( int k = 0; k < 3; k++ )
			v2[ k ] = vertices.get( indices.get( i + 2 ), VertexAttribute::Position, k );

		glm::vec3 e1, e2;
		e1 = glm::normalize( v1 - v0 );
		e2 = glm::normalize( v2 - v0 );

		glm::vec3 normal = glm::cross( e1, e2 );

		for( int j = 0; j < 3; j++ )
		{
			unsigned int index = indices.get( i + j );
			vertices.add( index, VertexAttribute::Normal, normal );
		}
	}

	for( unsigned int i = 0; i < vertices.getVerticesNumber(); i++ )
	{
		glm::vec3 normal;

		for( int j = 0; j < 3; j++ )
		{
			normal[ j ] = vertices.get( i, VertexAttribute::Normal, j );
		}

		normal = glm::normalize( normal );

		for( int j = 0; j < 3; j++ )
		{
			vertices.set( i, VertexAttribute::Normal, normal );
			//std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
		}
	}
}

float defaultHeightFunc( int, int )
{
	return 0.0f;
}