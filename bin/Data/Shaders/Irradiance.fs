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
	vec3 normal = normalize( Position );
	
	vec3 up = vec3( 0.0, 1.0, 0.0 );
	vec3 right = normalize( cross( normal, up ) );
	up = normalize( cross( right, normal ) );

	vec3 color = vec3( 0.0 );

	float index = 0.0;

	for( float phi = 0.0; phi < 6.283; phi += 0.025 )
	{
		for( float theta = 0.0; theta < 1.57; theta += 0.1 )
		{
			vec3 temp = cos( phi ) * right + sin( phi ) * up;
			temp = cos( theta ) * normal + sin( theta ) * temp;

			vec2 texCoords = getTexCoords( temp );
			//texCoords = vec2( phi / ( 2 * pi ), theta / pi );

			color += texture( probe, texCoords ).rgb * cos( theta ) * sin( theta );

			index++;
		}
	}

	color = pi * color / index;
	Color = vec4( color, 1.0 );
}