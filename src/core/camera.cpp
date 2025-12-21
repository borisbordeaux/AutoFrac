#include "core/camera.h"
#include <glm/ext/matrix_transform.hpp>

namespace Core {

Camera::Camera(const glm::vec3& center, const glm::vec3& upVector, float radius, float minRadius, float maxRadius, float azimuthAngle, float polarAngle) :
        m_center(center), m_upVector(upVector), m_radius(radius), m_minRadius(minRadius), m_maxRadius(maxRadius), m_azimuthAngle(azimuthAngle), m_polarAngle(polarAngle) {}

void Camera::rotateAzimuth(float radians) {
    m_azimuthAngle += radians;

    // Keep azimuth angle within range <0..2PI) - it's not necessary, just to have it nicely output
    const float fullCircle = 2.0f * 3.1415926f;
    m_azimuthAngle = fmodf(m_azimuthAngle, fullCircle);

    if (m_azimuthAngle < 0.0f) {
        m_azimuthAngle += fullCircle;
    }
}

void Camera::rotatePolar(float radians) {
    m_polarAngle += radians;

    // Check if the angle hasn't exceeded quarter of a circle to prevent flip, add a bit of epsilon like 0.001 radians
    const float polarMaxCap = 3.1415926f / 2.0f - 0.001f;
    const float polarMinCap = -3.1415926f / 2.0f + 0.001f;

    if (m_polarAngle > polarMaxCap) {
        m_polarAngle = polarMaxCap;
    }

    if (m_polarAngle < polarMinCap) {
        m_polarAngle = polarMinCap;
    }
}

void Camera::zoom(float by) {
    m_radius *= 1.0f - by;

    if (m_radius < m_minRadius) {
        m_radius = m_minRadius;
    }

    if (m_radius > m_maxRadius) {
        m_radius = m_maxRadius;
    }
}

void Camera::dezoom(float by) {
    m_radius /= 1.0f - by;

    if (m_radius < m_minRadius) {
        m_radius = m_minRadius;
    }

    if (m_radius > m_maxRadius) {
        m_radius = m_maxRadius;
    }
}

void Camera::moveHorizontal(float distance) {
    const glm::vec3 viewVector = getNormalizedViewVector();
    const glm::vec3 strafeVector = glm::normalize(glm::cross(viewVector, m_upVector));
    m_center += strafeVector * distance * 4.0f * m_radius / m_maxRadius;
}

void Camera::moveVertical(float distance) {
    const glm::vec3 viewVector = getNormalizedViewVector();
    const glm::vec3 strafeVector = glm::normalize(glm::cross(viewVector, m_upVector));
    const glm::vec3 upVector = glm::normalize(glm::cross(strafeVector, viewVector));
    m_center += upVector * distance * 4.0f * m_radius / m_maxRadius;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(getEye(), m_center, m_upVector);
}

glm::vec3 Camera::getEye() const {
    // Calculate sines / cosines of angles
    const float sineAzimuth = std::sin(m_azimuthAngle);
    const float cosineAzimuth = std::cos(m_azimuthAngle);
    const float sinePolar = std::sin(m_polarAngle);
    const float cosinePolar = std::cos(m_polarAngle);

    // Calculate eye position out of them
    const float x = m_center.x + m_radius * cosinePolar * cosineAzimuth;
    const float y = m_center.y + m_radius * sinePolar;
    const float z = m_center.z + m_radius * cosinePolar * sineAzimuth;

    return { x, y, z };
}

void Camera::reset(const glm::vec3& center, float radius, float azimuthAngle, float polarAngle) {
    m_center = center;
    m_radius = radius;
    m_azimuthAngle = azimuthAngle;
    m_polarAngle = polarAngle;
}

void Camera::setCenter(const glm::vec3& center) {
    m_center = center;
}

void Camera::setRadius(float radius) {
    m_radius = radius;
}

void Camera::setAzimuthAngle(float azimuthAngle) {
    m_azimuthAngle = azimuthAngle;
}

void Camera::setPolarAngle(float polarAngle) {
    m_polarAngle = polarAngle;
}

const glm::vec3& Camera::center() const {
    return m_center;
}

float Camera::radius() const {
    return m_radius;
}

float Camera::azimuthAngle() const {
    return m_azimuthAngle;
}

float Camera::polarAngle() const {
    return m_polarAngle;
}

glm::vec3 Camera::getNormalizedViewVector() const {
    return glm::normalize(m_center - getEye());
}

} // Core