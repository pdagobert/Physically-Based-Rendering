#include "Viewport.hpp"
#include <GL/glew.h>

Viewport::Viewport( const glm::ivec2& size, const glm::ivec2& position ) :
	m_size( size ),
	m_position( position )
{

}

void Viewport::bind()
{
	glViewport( m_position.x, m_position.y, m_size.x, m_size.y );
}