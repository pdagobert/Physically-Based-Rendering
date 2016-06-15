#version 330 core

uniform sampler2D colorBuffer;

uniform bool gamma;
uniform float exposure;
uniform bool tonemapping;

in vec2 TexCoords;

out vec4 Color;

vec3 reinhardTonemapping( vec3 col )
{
	return col / ( col + 1.0 );
}

void main()
{
	vec3 col = texture( colorBuffer, TexCoords ).rgb;
	col *= exposure;

	/*if( tonemapping )
		col = reinhardTonemapping( col );*/

	if( gamma )
		col = pow( col, vec3( 1.0 / 2.2 ) );

	Color = vec4( col, 1.0 );
}