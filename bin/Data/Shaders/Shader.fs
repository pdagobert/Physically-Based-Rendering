#version 330 core

uniform bool gamma;
uniform float exposure;
uniform bool tonemapping;

uniform int samples;

uniform samplerCube radiance;
uniform samplerCube irradiance;

uniform vec3 cameraPos;

struct Material
{
	bool hasAlbedoMap, hasNormalMap, hasAOMap, hasMetalnessMap, hasRoughnessMap;

	sampler2D albedoMap, normalMap, aoMap, metalnessMap, roughnessMap;

	vec3 albedo;
	float roughness;
	float metalness;

	float ior;
};

uniform Material material;

in vec3 Position;
in vec2 TexCoords;
in vec3 Normal;
in vec3 Tangent;

out vec4 Color;

const float pi = 3.1415926535897932384626433832795;

float radicalInverse_VdC( uint bits )
{
	bits = ( bits << 16u ) | ( bits >> 16u );
	bits = ( ( bits & 0x55555555u ) << 1u ) | ( ( bits & 0xAAAAAAAAu ) >> 1u );
	bits = ( ( bits & 0x33333333u ) << 2u ) | ( ( bits & 0xCCCCCCCCu ) >> 2u );
	bits = ( ( bits & 0x0F0F0F0Fu ) << 4u ) | ( ( bits & 0xF0F0F0F0u ) >> 4u );
	bits = ( ( bits & 0x00FF00FFu ) << 8u ) | ( ( bits & 0xFF00FF00u ) >> 8u );

	return float( bits ) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 hammersley2d( int i, int n )
{
	return vec2( float( i ) / float( n ), radicalInverse_VdC( uint( i ) ) );
}

vec3 importanceSamplingGGX( vec2 xi, float roughness, vec3 normal )
{
	float a = roughness * roughness;

	float phi = 2.0f * pi * xi.x;
	float costheta = sqrt( ( 1.0f - xi.y ) / ( 1.0 + ( a * a - 1.0f ) * xi.y ) );
	float sintheta = sqrt( 1 - costheta * costheta );

	vec3 localSpaceVector; // z pointing up
	localSpaceVector.x = sintheta * cos( phi );
	localSpaceVector.y = sintheta * sin( phi );
	localSpaceVector.z = costheta;

	vec3 up = abs( normal.z ) < 0.99999 ? vec3( 0.0, 0.0, 1.0 ) : vec3( 1.0, 0.0, 0.0 );
	vec3 tx = normalize( cross( up, normal ) );
	vec3 ty = normalize( cross( normal, tx ) );

	return tx * localSpaceVector.x + ty * localSpaceVector.y + normal * localSpaceVector.z;
}

float saturate( float x )
{
	return clamp( x, 0.0, 1.0 );
}

vec3 saturate( vec3 x )
{
	return clamp( x, 0.0, 1.0 );
}

vec3 fresnelSchlick( vec3 f0, float vdoth )
{
	return f0 + ( 1.0 - f0 ) * pow( 1.0 - vdoth, 5.0 );
}

vec3 fresnel( vec3 albedo, float metalness, float ior, float vdoth )
{
	vec3 f0;// = vec3( abs( 1.0 - ior / ( 1.0 + ior ) ) );
	f0 = vec3( ior );
	f0 *= f0;
	f0 = mix( f0, albedo, metalness );

	return fresnelSchlick( f0, vdoth );
}

float ggx( float ndotv, float a )
{
	float k = a / 2;
	return ndotv / ( ndotv * ( 1.0 - k ) + k );
}

float ggxGeometric( float ndotv, float ndotl, float roughness )
{
	return ggx( ndotv, roughness ) * ggx( ndotl, roughness );
}

vec3 reinhardTonemapping( vec3 col )
{
	return col / ( col + 1.0 );
}

vec3 getAlbedo()
{
	vec3 albedo;

	if( material.hasAlbedoMap )
		albedo = texture( material.albedoMap, TexCoords ).rgb;
	else
		albedo = material.albedo;

	return pow( albedo, vec3( 2.2 ) );
}

vec3 getNormal()
{
	if( !material.hasNormalMap )
		return normalize( Normal );

	vec3 n = normalize( Normal );
	vec3 t = normalize( Tangent );
	t = normalize( t - dot( t, n ) * n ); // re-orthogonalize T

	vec3 b = normalize( cross( t, n ) );

	vec3 sn = texture( material.normalMap, TexCoords ).rgb;
	sn = normalize( 2.0 * sn - 1.0 );
	//sn.y = -sn.y;

	mat3 tbn = mat3( t, b, n );

	return normalize( tbn * sn );
}

float getAO()
{
	if( material.hasAOMap )
		return texture( material.aoMap, TexCoords ).r;
	
	return 1.0;
}

float getMetalness()
{
	if( material.hasMetalnessMap )
		return texture( material.metalnessMap, TexCoords ).r;
	
	return material.metalness;
}

float getRoughness()
{
	if( material.hasRoughnessMap )
		return texture( material.roughnessMap, TexCoords ).r;
	
	return material.roughness;
}

void main()
{
	vec3 albedo = getAlbedo();
	//vec3 albedo = vec3( 0.549585, 0.556114, 0.554256 );

	float metalness = getMetalness();
	float roughness = getRoughness();

	vec3 normal = getNormal();
	vec3 viewDir = normalize( cameraPos - Position );

	vec3 specColor = vec3( 0.0 );

	vec3 ks = vec3( 0.0 );

	for( int i = 0; i < samples; i++ )
	{
		vec2 xi = hammersley2d( i, samples );

		vec3 h = importanceSamplingGGX( xi, roughness, normal );
		vec3 lightDir = normalize( reflect( -viewDir, h ) );

		float ndotl = saturate( dot( normal, lightDir ) );
		float vdoth = saturate( dot( viewDir, h ) );
		float ndotv = saturate( dot( normal, viewDir ) );
		float ndoth = saturate( dot( normal, h ) );

		if( ndotl > 0 )
		{
			float mips = 11.0;
			vec3 envColor = texture( radiance, lightDir, roughness * mips ).rgb;

			float G = ggxGeometric( ndotv, ndotl, roughness );
			vec3 F = fresnel( albedo, metalness, material.ior, vdoth );

			specColor += envColor * 2.0 * F * G * vdoth / ( ndotv * ndoth );

			ks += F;
		}
	}

	specColor /= samples;
	ks = saturate( ks / samples );

	vec3 diffColor = albedo * texture( irradiance, normal ).rgb * getAO();

	vec3 finalColor = diffColor * ( 1.0 - metalness ) + specColor;

	Color = vec4( finalColor, 1.0 );
}