#ifndef __LOADER_HPP__
#define __LOADER_HPP__

#include "OpenGL/Shader.hpp"
#include "OpenGL/Texture.hpp"
#include "VertexLayout.hpp"
#include "Mesh.hpp"
#include <string>

namespace Loader
{
	Shader loadShader( const std::string& vertexFile, const std::string& fragmentFile );

	std::string getShaderSource( const std::string fileName );
	std::string includeToSource( const std::string& source, const std::string& includeFile );

	Texture2D loadTexture2D( const std::string& fileName );
	CubeMap loadCubeMap( const std::string& path );
	Texture2D loadHdrTexture( const std::string& fileName );

	Mesh loadModel( const std::string& fileName, VertexLayout layout );
}
#endif