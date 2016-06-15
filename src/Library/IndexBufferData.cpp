#include "IndexBufferData.hpp"

IndexBufferData::IndexBufferData( int size ) :
	m_indices( size )
{

}

void IndexBufferData::set( int index, unsigned int value )
{
	m_indices[ index ] = value;
}

unsigned int IndexBufferData::get( int index ) const
{
	return m_indices[ index ];
}

unsigned int IndexBufferData::getSize() const
{
	return m_indices.size();
}

const std::vector< unsigned int >& IndexBufferData::getData() const
{
	return m_indices;
}