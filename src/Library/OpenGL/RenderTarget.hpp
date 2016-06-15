#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "Viewport.hpp"
#include "RenderCommand.hpp"
#include "FrameBuffer.hpp"
#include "Texture.hpp"
#include "RenderBuffer.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <map>

/*enum class RenderFlag
{
	Color = GL_COLOR_BUFFER_BIT,
	Depth = GL_DEPTH_BUFFER_BIT,
	Stencil = GL_STENCIL_BUFFER_BIT
};

inline RenderFlag operator|( RenderFlag a, RenderFlag b )
{
	return static_cast< RenderFlag >( static_cast< int >( a ) | static_cast< int >( b ) );
}*/

class RenderTarget
{
public:
	RenderTarget( const Viewport& viewport, /*RenderFlag flags,*/ bool isOffScreen );

	RenderTarget( RenderTarget&& target );
	RenderTarget& operator=( RenderTarget&& target );

	RenderTarget( const RenderTarget& ) = delete;
	RenderTarget& operator=( const RenderTarget& ) = delete;

	void attach( Attachment attachment, const Texture2D& texture );
	void attach( Attachment attachment, const CubeMap& cubemap, int face );
	void attach( Attachment attachment, const RenderBuffer& renderBuffer );

	void prepare();
	void draw( const RenderCommand& command );

private:
	//RenderFlag m_flags;
	Viewport m_viewport;

	std::unique_ptr< FrameBuffer > m_frameBuffer;
};
#endif