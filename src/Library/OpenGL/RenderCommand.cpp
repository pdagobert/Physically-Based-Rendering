#include "RenderCommand.hpp"

RenderCommand::RenderCommand()
{
	
}

RenderCommand::RenderCommand( Primitive primitive, unsigned int count, unsigned int offset, unsigned int instanceCount ) :
	primitive( primitive ),
	count( count ),
	offset( offset ),
	instanceCount( instanceCount )
{

}