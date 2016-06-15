#include "Application.hpp"
#include "Loader.hpp"
#include "VertexLayout.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <ATB/AntTweakBar.h>

const int windowWidth = 1440;
const int windowHeight = 900;


Application::Application() :
	Window( windowWidth, windowHeight, "OpenGL" ),
	m_camera( windowWidth, windowHeight, glm::vec3( 0.0f, 0.0f, 3.0f ), 1.0f ),
	m_target( Viewport( glm::ivec2( windowWidth, windowHeight ) ), true ),
	m_default( Viewport( glm::ivec2( windowWidth, windowHeight ) ), false ),
	m_framebufferTex( windowWidth, windowHeight, nullptr, InternalFormat::RGBFloat, PixelFormat::RGB, PixelType::Float ),
	m_framebufferDepth( windowWidth, windowHeight, Storage::Depth24 ),
	m_shader( Loader::loadShader( "Data/Shaders/Shader.vs", "Data/Shaders/Shader.fs" ) ),
	m_skyboxShader( Loader::loadShader( "Data/Shaders/Skybox.vs", "Data/Shaders/Skybox.fs" ) ),
	m_skydomeShader( Loader::loadShader( "Data/Shaders/Skydome.vs", "Data/Shaders/Skydome.fs" ) ),
	m_ppShader( Loader::loadShader( "Data/Shaders/Shader2.vs", "Data/Shaders/PostProcess.fs" ) )
{
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );

	VertexLayout layout;
	layout.addAttribute( VertexAttribute::Position, 3 );
	layout.addAttribute( VertexAttribute::TexCoords, 2 );
	layout.addAttribute( VertexAttribute::Normal, 3 );

	m_quad = Mesh( createUnitQuad( layout ), Primitive::Triangles );

	Texture2D probe = Loader::loadHdrTexture( "Data/EnvMap/PaperMill_E_3k.hdr" );
	probe.setWrap( TextureWrap::Clamp, TextureWrap::Clamp );
	probe.setFilter( TextureFilter::LinearMipMapLinear, TextureFilter::Linear );
	probe.generateMipMaps();

	m_radiances.push_back( createRadianceMap( 512, probe ) );
	m_irradiances.push_back( createIrradianceMap( 64, probe ) );

	m_framebufferTex.setWrap( TextureWrap::Clamp, TextureWrap::Clamp );
	m_framebufferTex.setFilter( TextureFilter::Linear, TextureFilter::Linear );

	m_target.attach( Attachment::Depth, m_framebufferDepth );
	m_target.attach( Attachment::Color0, m_framebufferTex );

	PBRInfo info;
	info.name = "Cerberus";
	info.hasAlbedoMap = true;
	info.hasNormalMap = true;
	info.hasAOMap = true;
	info.hasMetalnessMap = true;
	info.hasRoughnessMap = true;
	info.metalness = 0.0f;
	info.roughness = 0.0f;

	loadModel( info );

	setupUI();
}

void Application::setupUI()
{
	m_settings.wireframe = false;
	m_settings.gamma = true;
	m_settings.exposure = 1.0f;
	m_settings.tonemapping = true;
	m_settings.showRadiance = true;
	m_settings.samples = 16;
	m_settings.albedo = m_model.info.albedo;
	m_settings.roughness = m_model.info.roughness;
	m_settings.metalness = m_model.info.metalness;
	m_settings.ior = 0.04f;

	TwBar* bar = TwNewBar( "Tweaks" );

	TwAddVarRW( bar, "wireframe", TW_TYPE_BOOLCPP, &m_settings.wireframe, "group=settings" );
	TwAddVarRW( bar, "gamma", TW_TYPE_BOOLCPP, &m_settings.gamma, "group=settings" );
	TwAddVarRW( bar, "exposure", TW_TYPE_FLOAT, &m_settings.exposure, "group=settings" );
	TwAddVarRW( bar, "tonemapping", TW_TYPE_BOOLCPP, &m_settings.tonemapping, "group=settings" );
	TwAddVarRW( bar, "showRadiance", TW_TYPE_BOOLCPP, &m_settings.showRadiance, "group=settings" );

	TwAddVarRW( bar, "samples", TW_TYPE_INT32, &m_settings.samples, "group=settings min=0.0 max=2048" );

	TwAddVarRW( bar, "albedo", TW_TYPE_COLOR3F, &m_settings.albedo, "group=material" );
	TwAddVarRW( bar, "roughness", TW_TYPE_FLOAT, &m_settings.roughness, "group=material min=0.0 max=1.0 step=0.05" );
	TwAddVarRW( bar, "metalness", TW_TYPE_FLOAT, &m_settings.metalness, "group=material min=0.0 max=1.0 step=0.05" );
	TwAddVarRW( bar, "ior", TW_TYPE_FLOAT, &m_settings.ior, "group=material" );
}

void Application::loadModel( const PBRInfo& info )
{
	std::string path = "Data/Meshes/" + info.name + "/";

	VertexLayout layout;
	layout.addAttribute( VertexAttribute::Position, 3 );
	layout.addAttribute( VertexAttribute::TexCoords, 2 );
	layout.addAttribute( VertexAttribute::Normal, 3 );
	layout.addAttribute( VertexAttribute::Tangent, 3 );


	std::string ext = ".tga";

	//m_model.mesh = Mesh( createBox( glm::vec3( 1.0f ), layout ), Primitive::Triangles );
	m_model.mesh = Mesh( Loader::loadModel( path + info.name + ".FBX", layout ) );
	//m_model.mesh = Mesh( createUVSphere( 1.0f, 100, layout ), Primitive::Triangles );
	//m_model.mesh = Mesh( Loader::loadModel( path + info.name + ".obj", layout ) );

	if( info.hasAlbedoMap )
	{
		m_model.albedoMap = Loader::loadTexture2D( path + "albedo" + ext );
		m_model.albedoMap.setWrap( TextureWrap::Repeat, TextureWrap::Repeat );
		m_model.albedoMap.setFilter( TextureFilter::LinearMipMapLinear, TextureFilter::Linear );
		m_model.albedoMap.generateMipMaps();
	}

	if( info.hasNormalMap )
	{
		m_model.normalMap = Loader::loadTexture2D( path + "normal"  + ext );
		m_model.normalMap.setWrap( TextureWrap::Repeat, TextureWrap::Repeat );
		m_model.normalMap.setFilter( TextureFilter::LinearMipMapLinear, TextureFilter::Linear );
		m_model.normalMap.generateMipMaps();
	}

	if( info.hasAOMap )
	{
		m_model.aoMap = Loader::loadTexture2D( path + "ao" + ext );
		m_model.aoMap.setWrap( TextureWrap::Repeat, TextureWrap::Repeat );
		m_model.aoMap.setFilter( TextureFilter::LinearMipMapLinear, TextureFilter::Linear );
		m_model.aoMap.generateMipMaps();
	}

	if( info.hasMetalnessMap )
	{
		m_model.metalnessMap = Loader::loadTexture2D( path + "metalness" + ext );
		m_model.metalnessMap.setWrap( TextureWrap::Repeat, TextureWrap::Repeat );
		m_model.metalnessMap.setFilter( TextureFilter::LinearMipMapLinear, TextureFilter::Linear );
		m_model.metalnessMap.generateMipMaps();
	}

	if( info.hasRoughnessMap )
	{
		m_model.roughnessMap = Loader::loadTexture2D( path + "roughness" + ext );
		m_model.roughnessMap.setWrap( TextureWrap::Repeat, TextureWrap::Repeat );
		m_model.roughnessMap.setFilter( TextureFilter::LinearMipMapLinear, TextureFilter::Linear );
		m_model.roughnessMap.generateMipMaps();
	}

	m_model.info = info;
}

Skybox Application::createRadianceMap( int size, const Texture2D& probe )
{
	Skybox radiance( size, m_skyboxShader, true );

	Shader shader( Loader::loadShader( "Data/Shaders/Radiance.vs", "Data/Shaders/Radiance.fs" ) );
	RenderTarget target( Viewport( glm::ivec2( radiance.getSize(), radiance.getSize() ) ), true );

	glm::mat4 proj = glm::perspective( glm::radians( 90.0f ), 1.0f, 0.1f, 1000.0f );

	VertexLayout layout;
	layout.addAttribute( Position, 3 );
	layout.addAttribute( TexCoords, 2 );

	Mesh sphere = Mesh( createUVSphere( 1.0f, 100, layout ), Primitive::Triangles );
	
	glm::vec3 cameraDirs[] = 
	{
		glm::vec3( 1.0f, 0.0f, 0.0f ),
		glm::vec3( -1.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, 0.0f, 1.0f ),
		glm::vec3( 0.0f, 0.0f, -1.0f )
	};

	glm::vec3 cameraUps[] =
	{
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, 0.0f, 1.0f ),
		glm::vec3( 0.0f, 0.0f, -1.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f )
	};

	for( int i = 0; i < 6; i++ )
	{
		target.attach( Attachment::Color0, radiance.getCubeMap(), i );
		target.prepare();

		shader.bind();
		shader.setUniform( "probe", 0 );

		probe.bind( 0 );

		glm::mat4 view = glm::lookAt( glm::vec3( 0.0 ), cameraDirs[ i ], cameraUps[ i ] );
		shader.setUniform( "wvp", proj * view );

		sphere.render( target );
	}

	radiance.generateMipMaps();
	return radiance;
}

Skybox Application::createIrradianceMap( int size, const Texture2D& probe )
{
	Skybox irradiance( size, m_skyboxShader, true );

	Shader shader( Loader::loadShader( "Data/Shaders/Irradiance.vs", "Data/Shaders/Irradiance.fs" ) );
	RenderTarget target( Viewport( glm::ivec2( irradiance.getSize(), irradiance.getSize() ) ), true );

	glm::mat4 proj = glm::perspective( glm::radians( 90.0f ), 1.0f, 0.1f, 1000.0f );

	VertexLayout layout;
	layout.addAttribute( Position, 3 );
	
	Mesh sphere = Mesh( createUVSphere( 1.0f, 100, layout ), Primitive::Triangles );

	glm::vec3 cameraDirs[] = 
	{
		glm::vec3( 1.0f, 0.0f, 0.0f ),
		glm::vec3( -1.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, 0.0f, 1.0f ),
		glm::vec3( 0.0f, 0.0f, -1.0f )
	};

	glm::vec3 cameraUps[] =
	{
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, 0.0f, 1.0f ),
		glm::vec3( 0.0f, 0.0f, -1.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f )
	};

	for( int i = 0; i < 6; i++ )
	{
		target.attach( Attachment::Color0, irradiance.getCubeMap(), i );
		target.prepare();

		shader.bind();
		shader.setUniform( "probe", 0 );

		probe.bind( 0 );

		glm::mat4 view = glm::lookAt( glm::vec3( 0.0 ), cameraDirs[ i ], cameraUps[ i ] );
		shader.setUniform( "wvp", proj * view );

		sphere.render( target );
	}

	irradiance.generateMipMaps();
	return irradiance;
}

void Application::render( float )
{
	m_target.prepare();

	if( m_settings.wireframe )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	m_shader.bind();
	m_shader.setUniform( "wvp", m_camera.getMatrix() );
	m_shader.setUniform( "cameraPos", m_camera.getPosition() );
	m_shader.setUniform( "samples", m_settings.samples );

	m_shader.setUniform( "radiance", 8 );
	m_shader.setUniform( "irradiance", 1 );

	m_radiances[ 0 ].bind( 8 );
	m_irradiances[ 0 ].bind( 1 );

	setPBRUniforms( m_model, m_shader );

	glm::mat4 world;
	//world = glm::translate( world, glm::vec3( 0.0f, 0.0f, -15.0f ) );
	//world = glm::scale( world, glm::vec3( 5.0f ) );
	world = glm::rotate( world, glm::radians( 270.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	world = glm::rotate( world, glm::radians( -90.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
	world = glm::scale( world, glm::vec3( 0.01f ) );

	m_shader.setUniform( "world", world );

	m_model.mesh.render( m_target );

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	if( m_settings.showRadiance )
		m_radiances[ 0 ].render( m_target, m_camera );
	else
		m_irradiances[ 0 ].render( m_target, m_camera );

	renderPostProcess();
}

void Application::setPBRUniforms( const PBRModel& model, const Shader& shader )
{
	if( model.info.hasAlbedoMap )
	{
		shader.setUniform( "material.albedoMap", 2 );
		model.albedoMap.bind( 2 );
	}
	else
		shader.setUniform( "material.albedo", m_settings.albedo );

	if( model.info.hasNormalMap )
	{
		shader.setUniform( "material.normalMap", 3 );
		model.normalMap.bind( 3 );
	}

	if( model.info.hasAOMap )
	{
		shader.setUniform( "material.aoMap", 4 );
		model.aoMap.bind( 4 );
	}

	if( model.info.hasMetalnessMap )
	{
		shader.setUniform( "material.metalnessMap", 5 );
		model.metalnessMap.bind( 5 );
	}
	else
		shader.setUniform( "material.metalness", m_settings.metalness );

	if( model.info.hasRoughnessMap )
	{
		shader.setUniform( "material.roughnessMap", 6 );
		model.roughnessMap.bind( 6 );
	}
	else
		shader.setUniform( "material.roughness", m_settings.roughness );

	shader.setUniform( "material.hasAlbedoMap", (int)model.info.hasAlbedoMap );
	shader.setUniform( "material.hasNormalMap", (int)model.info.hasNormalMap );
	shader.setUniform( "material.hasAOMap", (int)model.info.hasAOMap );
	shader.setUniform( "material.hasMetalnessMap", (int)model.info.hasMetalnessMap );
	shader.setUniform( "material.hasRoughnessMap", (int)model.info.hasRoughnessMap );

	shader.setUniform( "material.ior", m_settings.ior );
}

void Application::renderPostProcess()
{
	m_default.prepare();

	m_ppShader.bind();
	m_ppShader.setUniform( "wvp", glm::mat4() );
	m_ppShader.setUniform( "world", glm::mat4() );
	m_ppShader.setUniform( "colorBuffer", 0 );

	m_ppShader.setUniform( "gamma", (int)m_settings.gamma );
	m_ppShader.setUniform( "exposure", m_settings.exposure );
	//m_ppShader.setUniform( "tonemapping", (int)m_settings.tonemapping );

	m_framebufferTex.bind( 0 );

	m_quad.render( m_default );

	TwDraw();
}

void Application::processEvents( float deltaTime )
{
	if( isKeyPressed( GLFW_KEY_LEFT ) )
		m_camera.moveLeft( deltaTime );

	if( isKeyPressed( GLFW_KEY_RIGHT ) )
		m_camera.moveRight( deltaTime );

	if( isKeyPressed( GLFW_KEY_UP ) )
		m_camera.moveForward( deltaTime );

	if( isKeyPressed( GLFW_KEY_DOWN ) )
		m_camera.moveBackward( deltaTime );
}

void Application::mouseMoved( float x, float y )
{
	static float xOld = x, yOld = y;

	if( isButtonPressed( GLFW_MOUSE_BUTTON_RIGHT ) )
		m_camera.orientate( x - xOld, yOld - y );

	xOld = x;
	yOld = y;
}