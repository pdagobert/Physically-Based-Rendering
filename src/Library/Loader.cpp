#include "Loader.hpp"
#include "VertexBufferData.hpp"
#include "IndexBufferData.hpp"
#include "OpenGL/RenderCommand.hpp"
#include <fstream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <utility>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

namespace Loader
{
	Shader loadShader( const std::string& vertexFile, const std::string& fragmentFile )
	{
		std::vector< GL::Shader > shaders;
		shaders.push_back( GL::Shader( GL_VERTEX_SHADER, vertexFile, getShaderSource( vertexFile ) ) );
		shaders.push_back( GL::Shader( GL_FRAGMENT_SHADER, fragmentFile, getShaderSource( fragmentFile ) ) );

		return Shader( std::move( shaders ) );
	}

	std::string getShaderSource( const std::string fileName )
	{
		std::ifstream file( fileName );
		if( file.fail() )
			throw std::runtime_error( "Shader file not found : " + fileName );

		std::string source;

		std::string line;
		while( std::getline( file, line ) )
		{
			if( line.find( "#include" ) != std::string::npos )
			{
				int beginPos = line.find( "\"" ) + 1;
				int endPos = line.find( "\"", beginPos );

				int size = endPos - beginPos;

				std::string includeFile = line.substr( beginPos, size );
				source = includeToSource( source, includeFile );
			}
			else
				source += line + '\n';
		}
		
		return source;
	}

	std::string includeToSource( const std::string& source, const std::string& includeFile )
	{
		std::ifstream file( includeFile );
		if( file.fail() )
		{
			throw std::runtime_error( source + " : include file not found (" + includeFile + ")" );
		}

		std::stringstream ss;
		ss << file.rdbuf();

		return source + '\n' + ss.str() + '\n';
	}

	Texture2D loadTexture2D( const std::string& fileName )
	{
		int width, height, channels;
		unsigned char* data = SOIL_load_image( fileName.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA );
		if( !data )
			throw std::runtime_error( "Texture not found : " + fileName );

		Texture2D texture( width, height, data, InternalFormat::RGBA, PixelFormat::RGBA, PixelType::UByte );
		SOIL_free_image_data( data );

		return texture;
	}

	static const std::vector< std::string > prefixes = { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "back.jpg", "front.jpg" };

	CubeMap loadCubeMap( const std::string& path )
	{
		std::vector< const void* > pixels( 6 );

		int size;

		for( unsigned int i = 0; i < pixels.size(); i++ )
		{
			std::string fileName = path + "/" + prefixes[ i ];

			int width, height, channels;
			unsigned char* data = SOIL_load_image( fileName.c_str(), &width, &height, &channels, SOIL_LOAD_RGB );
			if( !data )
				throw std::runtime_error( "Texture not found : " + fileName );

			pixels[ i ] = data;

			size = width;
		}

		CubeMap cubemap( size, pixels, InternalFormat::RGB, PixelFormat::RGB, PixelType::UByte );

		for( unsigned int i = 0; i < pixels.size(); i++ )
			SOIL_free_image_data( (unsigned char*)pixels[ i ] );

		return cubemap;
	}

	Texture2D loadHdrTexture( const std::string& fileName )
	{
		int width, height;
		float* data = SOIL_load_hdr_image( fileName.c_str(), &width, &height );
		if( !data )
			throw std::runtime_error( "Probe not found : " + fileName );

		Texture2D texture( width, height, data, InternalFormat::RGBFloat, PixelFormat::RGB, PixelType::Float );

		SOIL_free_hdr_image_data( data );
		return texture;
	}

	Mesh loadModel( const std::string& fileName, VertexLayout layout )
	{		
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile( fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs );
		if( !scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
			throw std::runtime_error( "Assimp model loading failed : " + std::string( importer.GetErrorString() ) );

		importer.ApplyPostProcessing( aiProcess_CalcTangentSpace );

		//for( unsigned int k = 0; k < scene->mNumMeshes; k++ )
		{
			aiMesh* mesh = scene->mMeshes[ 0 ];

			VertexBufferData vertices( layout, mesh->mNumVertices );

			int index = 0;

			for( unsigned int i = 0; i < mesh->mNumVertices; i++ )
			{
				vertices.set( index, VertexAttribute::Position, glm::vec3( mesh->mVertices[ i ].x, mesh->mVertices[ i ].y, mesh->mVertices[ i ].z ) );

				if( layout.has( VertexAttribute::TexCoords ) && mesh->HasTextureCoords( 0 ) )
					vertices.set( index, VertexAttribute::TexCoords, glm::vec2( mesh->mTextureCoords[ 0 ][ i ].x, mesh->mTextureCoords[ 0 ][ i ].y ) );

				if( layout.has( VertexAttribute::Normal ) )
					vertices.set( index, VertexAttribute::Normal, glm::vec3( mesh->mNormals[ i ].x, mesh->mNormals[ i ].y, mesh->mNormals[ i ].z ) );

				if( layout.has( VertexAttribute::Tangent ) && mesh->HasTangentsAndBitangents() )
					vertices.set( index, VertexAttribute::Tangent, glm::vec3( mesh->mTangents[ i ].x, mesh->mTangents[ i ].y, mesh->mTangents[ i ].z ) );

				index++;
			}

			index = 0;
			IndexBufferData indices( mesh->mNumFaces * 3 );

			for( unsigned int i = 0; i < mesh->mNumFaces; i++ )
			{
				aiFace face = mesh->mFaces[ i ];

				for( unsigned int j = 0; j < face.mNumIndices; j++ )
				{
					indices.set( index, face.mIndices[ j ] );
					index++;
				}
			}

			Geometry g( vertices, indices );

			return Mesh( g, Primitive::Triangles );
		}
	}
}