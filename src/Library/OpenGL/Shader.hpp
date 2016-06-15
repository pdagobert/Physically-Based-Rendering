#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace GL
{
	class Program;

	class Shader
	{
	public:
		Shader();
		Shader( GLenum type, const std::string& fileName, const std::string& source );
		~Shader();

		Shader( Shader&& copy );
		Shader& operator=( Shader&& copy );

		Shader( const Shader& ) = delete;
		Shader& operator=( const Shader& ) = delete;

		void setSource( const std::string& source );
		void compile();

		void setProgram( Program& program );

		GLuint getHandle() const;

	private:
		std::string m_fileName;
		GLuint m_handle;
		Program* m_program;
	};

	class Program
	{
	public:
		Program();
		~Program();

		Program( Program&& copy );
		Program& operator=( Program&& copy );

		Program( const Program& ) = delete;
		Program& operator=( const Program& ) = delete;

		void attach( Shader& shader );
		void detach( Shader& shader );

		void link();
		void use() const;

		GLint getUniformLocation( const std::string& name ) const;

	private:
		GLuint m_handle;
	};
}

class Shader
{
public:
	Shader();
	Shader( std::vector< GL::Shader > shaders );

	Shader( Shader&& copy );
	Shader& operator=( Shader&& copy );

	Shader( const Shader& ) = delete;
	Shader& operator=( const Shader& ) = delete;

	void bind() const;

	void setUniform( const std::string& name, const glm::mat4& value ) const;
	void setUniform( const std::string& name, const glm::mat3& value ) const;
	void setUniform( const std::string& name, const glm::vec4& value ) const;
	void setUniform( const std::string& name, const glm::vec3& value ) const;
	void setUniform( const std::string& name, const glm::vec2& value ) const;
	void setUniform( const std::string& name, float value ) const;
	void setUniform( const std::string& name, int value ) const;

private:
	GL::Program m_program;
};
#endif