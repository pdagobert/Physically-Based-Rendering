#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm/glm.hpp>

class Camera
{
public:
	Camera( int windowWidth, int windowHeight, glm::vec3 position = glm::vec3( 0.0f, 0.0f, 1.0f ), float speed = 1.0f );

	void moveLeft( float deltaTime );
	void moveRight( float deltaTime );
	void moveForward( float deltaTime );
	void moveBackward( float deltaTime );

	void orientate( float x, float y );

	void setSpeed( float speed );

	glm::mat4 getMatrix() const;
	const glm::mat4& getProjectionMatrix() const;
	const glm::mat4& getViewMatrix() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getFront() const;

private:
	void updateOrientation();
	void updateMatrix();

	void setProjection( int windowWidth, int windowHeight );

private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	float m_pitch, m_yaw;

	float m_speed, m_sensitivity;

	glm::mat4 m_viewMatrix, m_projectionMatrix;
};
#endif