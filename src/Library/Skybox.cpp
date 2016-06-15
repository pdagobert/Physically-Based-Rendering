#include "Skybox.hpp"
#include "OpenGL/Shader.hpp"
#include "OpenGL/RenderTarget.hpp"
#include "OpenGL/Camera.hpp"
#include "Loader.hpp"
#include "Geometry.hpp"
#include "VertexLayout.hpp"

Skybox::Skybox( int size, const Shader& shader, bool hdr ) :
	m_size( size ),
	m_shader( shader )
{
	if( hdr )
		m_cubeMap = CubeMap( size, std::vector< const void* >( 6, nullptr ), InternalFormat::RGBFloat, PixelFormat::RGB, PixelType::Float );
	else
		m_cubeMap = CubeMap( size, std::vector< const void* >( 6, nullptr ), InternalFormat::RGB, PixelFormat::RGB, PixelType::UByte );

	m_cubeMap.setWrap( TextureWrap::Clamp, TextureWrap::Clamp, TextureWrap::Clamp );
	m_cubeMap.setFilter( TextureFilter::Linear, TextureFilter::Linear );

	VertexLayout layout;
	layout.addAttribute( VertexAttribute::Position, 3 );
	m_cube = Mesh( createBox( glm::vec3( 1.0f ), layout ), Primitive::Triangles );
}

Skybox::Skybox( const std::string& path, const Shader& shader ) :
	m_shader( shader ),
	m_cubeMap( Loader::loadCubeMap( path ) )
{	
	m_cubeMap.setWrap( TextureWrap::Clamp, TextureWrap::Clamp, TextureWrap::Clamp );
	m_cubeMap.setFilter( TextureFilter::LinearMipMapLinear, TextureFilter::Linear );
	m_cubeMap.generateMipMaps();
	m_cubeMap.setAnisotropicValue( 16.0f );

	VertexLayout layout;
	layout.addAttribute( VertexAttribute::Position, 3 );

	m_cube = Mesh( createBox( glm::vec3( 1.0f ), layout ), Primitive::Triangles );
}

void Skybox::render( RenderTarget& target, const Camera& camera )
{
	m_shader.bind();
	m_shader.setUniform( "wvp", camera.getProjectionMatrix() * glm::mat4( glm::mat3( camera.getViewMatrix() ) ) );
	m_shader.setUniform( "skybox", 0 );

	m_cubeMap.bind( 0 );

	m_cube.render( target );
}

void Skybox::bind( int unit ) const
{
	m_cubeMap.bind( unit );
}

const CubeMap& Skybox::getCubeMap() const
{
	return m_cubeMap;
}

int Skybox::getSize() const
{
	return m_size;
}

void Skybox::generateMipMaps()
{
	m_cubeMap.setFilter( TextureFilter::LinearMipMapLinear, TextureFilter::Linear );
	m_cubeMap.generateMipMaps();
	m_cubeMap.setAnisotropicValue( 16.0f );
}