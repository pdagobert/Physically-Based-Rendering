#ifndef __VIEWPORT_HPP__
#define __VIEWPORT_HPP__

#include <glm/glm.hpp>

class Viewport
{
public:
	Viewport( const glm::ivec2& size, const glm::ivec2& position = glm::ivec2( 0 ) );

	void bind();

private:
	glm::ivec2 m_size;
	glm::ivec2 m_position;
};
#endif