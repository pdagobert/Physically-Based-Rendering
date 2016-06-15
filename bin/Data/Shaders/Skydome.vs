#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec2 texCoords;

uniform mat4 wvp;

out vec2 TexCoords;

const float pi = 3.1415926535897932384626433832795;

void main()
{
	gl_Position = wvp * vec4( position * 1.0, 1.0 );
	gl_Position = gl_Position.xyww;

	TexCoords = texCoords;
}