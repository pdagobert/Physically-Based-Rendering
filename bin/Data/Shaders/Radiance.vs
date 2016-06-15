#version 330 core

layout( location = 0 ) in vec3 position;

uniform mat4 wvp;

out vec3 Position;

void main()
{
	gl_Position = wvp * vec4( position, 1.0 );

	Position = position;
}