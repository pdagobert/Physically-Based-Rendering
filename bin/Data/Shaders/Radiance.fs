#version 330 core

uniform int face;
uniform sampler2D probe;

in vec3 Position;

out vec4 Color;

const float pi = 3.1415926535897932384626433832795;

vec2 getTexCoords( vec3 pos )
{
	float phi = acos( pos.y );
	float theta = atan( -1 * pos.x, pos.z ) + pi;

	return vec2( theta / ( 2 * pi ), phi / pi );
}

void main()
{
	vec3 normal;

	/*if( face == 0 )
		normal = vec3( 1.0, Position.y, -Position.x );
	else if( face == 1 )
		normal = vec3( -1.0, Position.y, Position.x );
	else if( face == 2 )
		normal = vec3( Position.x, -1.0, Position.y );
	else if( face == 3 )
		normal = vec3( Position.x, 1.0, -Position.y );
	else if( face == 4 )
		normal = vec3( Position.x, Position.y, 1.0 );
	else if( face == 5 )
		normal = vec3( -Position.x, Position.y, -1.0 );*/

	//normal = mapCubeToSphere( normal );

	/*vec3 r = Position;//normalize(  );
	float m = 2 * sqrt( r.x * r.x + r.y * r.y + ( r.z + 1 ) * ( r.z + 1 ) );

	vec2 texCoords = r.xy / m + 0.5;*/

	
	vec3 col = texture( probe, getTexCoords( normalize( Position ) ) ).rgb;

	Color = vec4( col, 1.0 );
}