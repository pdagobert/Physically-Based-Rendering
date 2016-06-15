#version 330 core

uniform samplerCube skybox;

in vec3 TexCoords;

out vec4 Color;

void main()
{
	vec3 texColor = texture( skybox, TexCoords ).rgb;
	Color = vec4( texColor, 1.0 );
}