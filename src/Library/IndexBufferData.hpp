#ifndef __INDEX_BUFFER_DATA_HPP__
#define __INDEX_BUFFER_DATA_HPP__

#include <vector>

class IndexBufferData
{
public:
	IndexBufferData( int size );

	void set( int index, unsigned int value );
	unsigned int get( int index ) const;

	unsigned int getSize() const;
	const std::vector< unsigned int >& getData() const;

private:
	std::vector< unsigned int > m_indices;
};
#endif