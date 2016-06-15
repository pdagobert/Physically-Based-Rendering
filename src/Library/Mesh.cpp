#include "Mesh.hpp"
#include "OpenGL/RenderTarget.hpp"

Mesh::Mesh() :
	m_vb(),
	m_ib(),
	m_va(),
	m_command()
{

}

Mesh::Mesh( Geometry g, Primitive primitive ) :
	m_vb(),
	m_ib(),
	m_va(),
	m_command( primitive, g.indices.getData().size(), 0 )
{
	m_vb.setData( g.vertices.getData(), Usage::Static );
	m_ib.setData( g.indices.getData(), Usage::Static );

	m_va.bind();

	m_vb.bind();
	g.vertices.bind();

	m_ib.bind();

	m_va.unbind();

	m_vb.unbind();
	m_ib.unbind();
}

Mesh::Mesh( Mesh&& mesh ) :
	m_vb( std::move( mesh.m_vb ) ),
	m_ib( std::move( mesh.m_ib ) ),
	m_va( std::move( mesh.m_va ) ),
	m_command( mesh.m_command )
{

}

Mesh& Mesh::operator=( Mesh&& mesh )
{
	m_vb = std::move( mesh.m_vb );
	m_ib = std::move( mesh.m_ib );
	m_va = std::move( mesh.m_va );
	m_command = mesh.m_command;

	return *this;
}

void Mesh::render( RenderTarget& target )
{
	m_va.bind();
	target.draw( m_command );
	m_va.unbind();
}