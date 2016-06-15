#ifndef __SKYBOX_HPP__
#define __SKYBOX_HPP__

#include "OpenGL/Texture.hpp"
#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <string>

class Shader;
class RenderTarget;
class Camera;

class Skybox
{
public:
	Skybox( int size, const Shader& shader, bool hdr = false );
	Skybox( const std::string& path, const Shader& shader );

	void render( RenderTarget& target, const Camera& camera );

	void bind( int unit ) const;

	const CubeMap& getCubeMap() const;

	int getSize() const;

	void generateMipMaps();

private:
	int m_size;
	
	const Shader& m_shader;

	Mesh m_cube;

	CubeMap m_cubeMap;
};
#endif