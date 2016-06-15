#ifndef __RENDER_COMMAND_HPP__
#define __RENDER_COMMAND_HPP__

#include <GL/glew.h>

enum class Primitive
{
	Triangles = GL_TRIANGLES,
	Points = GL_POINTS
};

class RenderCommand
{
public:
	RenderCommand();
	RenderCommand( Primitive primitive, unsigned int count, unsigned int offset, unsigned int instanceCount = 0 );

public:
	Primitive primitive;
	unsigned int count;
	unsigned int offset;

	unsigned int instanceCount;
};
#endif