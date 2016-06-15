#ifndef __SKY_DOME_HPP__
#define __SKY_DOME_HPP__

#include "OpenGL/Texture.hpp"
#include "Mesh.hpp"
#include <string>

class Shader;
class RenderTarget;
class Camera;

class SkyDome
{
public:
	SkyDome( const std::string& fileName, const Shader& shader );

	void render( RenderTarget& target, const Camera& camera );
	void bind( int unit ) const;
	
private:
	const Shader& m_shader;

	Texture2D m_tex;
	Mesh m_sphere;
};
#endif