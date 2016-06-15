#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec2 texCoords;
layout( location = 2 ) in vec3 normal;
layout( location = 3 ) in vec3 tangent;

uniform mat4 wvp;
uniform mat4 world;

out vec3 Position;
out vec2 TexCoords;
out vec3 Normal;
out vec3 Tangent;

void main()
{
	gl_Position = wvp * world * vec4( position, 1.0 );

	Position = vec3( world * vec4( position, 1.0 ) );
	TexCoords = texCoords;
	Normal = mat3( transpose( inverse( world ) ) ) * normal;
	Tangent = mat3( transpose( inverse( world ) ) ) * tangent;
}