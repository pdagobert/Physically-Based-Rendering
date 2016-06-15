#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include "OpenGL/Window.hpp"
#include "OpenGL/Shader.hpp"
#include "OpenGL/Camera.hpp"
#include "OpenGL/RenderTarget.hpp"
#include "OpenGL/Texture.hpp"
#include "OpenGL/RenderBuffer.hpp"
#include "Mesh.hpp"
#include "Skybox.hpp"
#include "SkyDome.hpp"
#include <vector>

struct Settings
{
	bool wireframe;
	bool gamma;
	float exposure;
	bool tonemapping;
	bool showRadiance;

	int samples;

	glm::vec3 albedo;
	float roughness, metalness;
	float ior;
};

struct PBRInfo
{
	std::string name;
	bool hasAlbedoMap, hasNormalMap, hasAOMap, hasMetalnessMap, hasRoughnessMap;

	glm::vec3 albedo = glm::vec3( 1.0f );

	float metalness = 0.0f;
	float roughness = 0.0f;
};

struct PBRModel
{
	PBRInfo info;

	Mesh mesh;

	Texture2D albedoMap, normalMap, roughnessMap, metalnessMap, aoMap;
};

class Application : public Window
{
public:
	Application();

	virtual void render( float deltaTime );

	virtual void processEvents( float deltaTime );
	virtual void mouseMoved( float x, float y );

private:
	void setupUI();
	void loadModel( const PBRInfo& info );

	void setPBRUniforms( const PBRModel& model, const Shader& shader );

	void renderPostProcess();

	Skybox createRadianceMap( int size, const Texture2D& probe );
	Skybox createIrradianceMap( int size, const Texture2D& probe );

private:
	Camera m_camera;
	RenderTarget m_target, m_default;

	RenderBuffer m_framebufferDepth;
	Texture2D m_framebufferTex;

	Shader m_shader, m_skyboxShader, m_skydomeShader, m_ppShader;

	std::vector< Skybox > m_radiances;
	std::vector< Skybox > m_irradiances;

	Mesh m_quad;

	Texture2D m_probe;

	Settings m_settings;

	PBRModel m_model;

};
#endif