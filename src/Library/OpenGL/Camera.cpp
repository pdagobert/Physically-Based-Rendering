#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera( int windowWidth, int windowHeight, glm::vec3 position, float speed ) :
	m_position( position ),
	m_pitch( 0.0f ),
	m_yaw( -90.0f ),
	m_speed( speed ),
	m_sensitivity( 1.0f )
{
	setProjection( windowWidth, windowHeight );
	updateOrientation();
	updateMatrix();
}

void Camera::moveLeft( float deltaTime )
{
	glm::vec3 right = glm::cross( m_front, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	right = glm::normalize( right );

	m_position -= right * m_speed * deltaTime;
	updateMatrix();
}

void Camera::moveRight( float deltaTime )
{
	glm::vec3 right = glm::cross( m_front, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	right = glm::normalize( right );

	m_position += right * m_speed * deltaTime;
	updateMatrix();
}

void Camera::moveForward( float deltaTime )
{
	glm::vec3 front = m_front;
	//front.y = 0.0f;
	front = glm::normalize( front );

	m_position += front * m_speed * deltaTime;
	updateMatrix();
}

void Camera::moveBackward( float deltaTime )
{
	glm::vec3 front = m_front;
	//front.y = 0.0f;
	front = glm::normalize( front );

	m_position -= front * m_speed * deltaTime;
	updateMatrix();
}

void Camera::orientate( float x, float y )
{
	m_pitch += y;
	m_yaw += x;

	m_pitch = glm::clamp( m_pitch, -89.0f, 89.0f );

	updateOrientation();
	updateMatrix();
}

void Camera::updateOrientation()
{
	float pitchRad = glm::radians( m_pitch );
	float yawRad = glm::radians( m_yaw );

	m_front.x = glm::cos( pitchRad ) * glm::cos( yawRad );
	m_front.y = glm::sin( pitchRad );
	m_front.z = glm::cos( pitchRad ) * glm::sin( yawRad );
}

void Camera::setProjection( int windowWidth, int windowHeight )
{
	float aspect = (float)windowWidth / windowHeight;
	m_projectionMatrix = glm::perspective( glm::radians( 45.0f ), aspect, 0.1f, 10000.0f );
}

void Camera::updateMatrix()
{
	m_viewMatrix = glm::lookAt( m_position, m_position + m_front, glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void Camera::setSpeed( float speed )
{
	m_speed = speed;
}

glm::mat4 Camera::getMatrix() const
{
	return m_projectionMatrix * m_viewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}

const glm::mat4& Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

const glm::vec3& Camera::getPosition() const
{
	return m_position;
}

const glm::vec3& Camera::getFront() const
{
	return m_front;
}
