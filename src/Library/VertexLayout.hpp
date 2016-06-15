#ifndef __VERTEX_LAYOUT_HPP__
#define __VERTEX_LAYOUT_HPP__

#include <GL/glew.h>
#include <map>

enum VertexAttribute
{
	Position = 0x1,
	TexCoords = 0x2,
	Normal = 0x4,
	Tangent = 0x8
};

struct Attribute
{
	int size;
	int offset;
};

class VertexLayout
{
public:
	VertexLayout();

	void addAttribute( int id, int size );

	int has( int id ) const;
	int getSize() const;
	int getOffset( int id ) const;
	const std::map< int, Attribute >& getAttributes() const;

private:
	int m_size;
	int m_attributeFlags;
	std::map< int, Attribute > m_attributes;
};
#endif