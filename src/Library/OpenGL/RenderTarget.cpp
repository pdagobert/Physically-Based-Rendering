#include "RenderTarget.hpp"
#include <utility>
#include <stdexcept>

RenderTarget::RenderTarget( const Viewport& viewport, /*RenderFlag flags,*/ bool isOffScreen ) :
	m_viewport( viewport )
	//m_flags( flags )
{
	if( isOffScreen )
	{
		m_frameBuffer = std::unique_ptr< FrameBuffer >( new FrameBuffer() );//std::make_unique( FrameBuffer );//
		// check flags and determin
	}
}

RenderTarget::RenderTarget( RenderTarget&& target ) :
	m_viewport( target.m_viewport ),
	m_frameBuffer( std::move( target.m_frameBuffer ) )
{

}

RenderTarget& RenderTarget::operator=( RenderTarget&& target )
{
	m_viewport = target.m_viewport;
	m_frameBuffer = std::move( target.m_frameBuffer );
	return *this;
}

void RenderTarget::attach( Attachment attachment, const Texture2D& texture )
{
	if( m_frameBuffer == nullptr )
		throw std::runtime_error( "Cannot attach target on default framebuffer" );

	m_frameBuffer->attach( attachment, texture );
}

void RenderTarget::attach( Attachment attachment, const CubeMap& cubemap, int face )
{
	if( m_frameBuffer == nullptr )
		throw std::runtime_error( "Cannot attach target on default framebuffer" );

	m_frameBuffer->attach( attachment, cubemap, face );
}

void RenderTarget::attach( Attachment attachment, const RenderBuffer& renderBuffer )
{
	if( m_frameBuffer == nullptr )
		throw std::runtime_error( "Cannot attach target on default framebuffer" );

	m_frameBuffer->attach( attachment, renderBuffer );
}

void RenderTarget::prepare()
{
	m_viewport.bind();

	if( m_frameBuffer )
		m_frameBuffer->bind();
	else
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	//glClear( static_cast< GLbitfield >( m_flags ) );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void RenderTarget::draw( const RenderCommand& command )
{
	if( command.instanceCount != 0 )
		glDrawElementsInstanced( static_cast< GLenum >( command.primitive ), command.count, GL_UNSIGNED_INT, reinterpret_cast< const GLvoid* >( command.offset * 4 ), command.instanceCount );
	else
		glDrawElements( static_cast< GLenum >( command.primitive ), command.count, GL_UNSIGNED_INT, reinterpret_cast< const GLvoid* >( command.offset * 4 ) );
}