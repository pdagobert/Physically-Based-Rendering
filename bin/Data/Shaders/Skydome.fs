#version 330 core

uniform sampler2D tex;

in vec2 TexCoords;

out vec4 Color;

void main()
{
	vec3 texColor = texture( tex, TexCoords ).rgb;
	Color = vec4( texColor, 1.0 );
}