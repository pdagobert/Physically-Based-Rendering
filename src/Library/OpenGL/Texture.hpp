#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <GL/glew.h>
#include <vector>

enum class InternalFormat
{
	Alpha = GL_RED,
	RGB = GL_RGB,
	RGBA = GL_RGBA,
	AlphaFloat = GL_R32F,
	RGBFloat = GL_RGB32F
};

enum class PixelFormat
{
	Alpha = GL_RED,
	RGB = GL_RGB,
	RGBA = GL_RGBA
};

enum class PixelType
{
	UByte = GL_UNSIGNED_BYTE,
	Float = GL_FLOAT
};

enum class TextureWrap
{
	Clamp = GL_CLAMP_TO_EDGE,
	Repeat = GL_REPEAT,
	Mirrored = GL_MIRRORED_REPEAT,
};

enum class TextureFilter
{
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
	LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR
};

class Texture
{
public:
	virtual ~Texture();
};

class Texture2D : public Texture
{
public:
	Texture2D();
	Texture2D( int width, int height, const void* pixels, InternalFormat internalFormat, PixelFormat format, PixelType pixelType );
	virtual ~Texture2D();

	Texture2D( Texture2D&& texture );
	Texture2D& operator=( Texture2D&& texture );

	void setWrap( TextureWrap wrapS, TextureWrap wrapT );
	void setFilter( TextureFilter minFilter, TextureFilter magFilter );

	void generateMipMaps();
	void setAnisotropicValue( float value );

	void bind( int unit = 0 ) const;

	GLuint getHandle() const;
	int getWidth() const;
	int getHeight() const;

private:
	GLuint m_handle;

	int m_width, m_height;
};

class CubeMap
{
public:
	CubeMap();
	CubeMap( int size, const std::vector< const void* >& pixels, InternalFormat internalFormat, PixelFormat format, PixelType pixelType );
	~CubeMap();

	CubeMap( CubeMap&& cubeMap );
	CubeMap& operator=( CubeMap&& cubeMap );

	void setWrap( TextureWrap wrapS, TextureWrap wrapT, TextureWrap wrapR );
	void setFilter( TextureFilter minFilter, TextureFilter magFilter );

	void generateMipMaps();
	void setAnisotropicValue( float value );

	void bind( int unit = 0 ) const;

	GLuint getHandle() const;

private:
	GLuint m_handle;
};
#endif