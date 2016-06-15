#include "VertexLayout.hpp"
#include <stdexcept>

VertexLayout::VertexLayout() :
	m_size( 0 ),
	m_attributeFlags( 0 ),
	m_attributes()
{

}

void VertexLayout::addAttribute( int id, int size )
{
	Attribute attrib = { size, m_size };

	m_attributes[ id ] = attrib;
	m_size += size;

	m_attributeFlags |= id;
}

int VertexLayout::has( int id ) const
{
	return m_attributeFlags & id;
}

int VertexLayout::getOffset( int id ) const
{
	auto it = m_attributes.find( id );
	if( it == m_attributes.end() )
		throw std::runtime_error( "Attribute not found : " );

	return it->second.offset;
}

int VertexLayout::getSize() const
{
	return m_size;
}

const std::map< int, Attribute >& VertexLayout::getAttributes() const
{
	return m_attributes;
}