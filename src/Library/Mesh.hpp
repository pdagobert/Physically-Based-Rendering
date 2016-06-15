#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "Geometry.hpp"
#include "OpenGL/Buffer.hpp"
#include "OpenGL/VertexArray.hpp"
#include "OpenGL/RenderCommand.hpp"

class RenderTarget;

class Mesh
{
public:
	Mesh();
	Mesh( Geometry g, Primitive primitive );

	Mesh( Mesh&& mesh );
	Mesh& operator=( Mesh&& mesh );

	Mesh( const Mesh& mesh ) = delete;
	Mesh& operator=( const Mesh& mesh ) = delete;

	void render( RenderTarget& target );

private:
	VertexBuffer m_vb;
	IndexBuffer m_ib;

	VertexArray m_va;
	RenderCommand m_command;
};
#endif