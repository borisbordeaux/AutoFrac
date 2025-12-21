#ifndef AUTOFRAC_CAMERA_H
#define AUTOFRAC_CAMERA_H

#include <glm/glm.hpp>

namespace Core {

class Camera {
public:
    Camera(const glm::vec3& center, const glm::vec3& upVector, float radius, float minRadius, float maxRadius, float azimuthAngle, float polarAngle);
    void rotateAzimuth(float radians);
    void rotatePolar(float radians);
    void zoom(float by = 0.1f);
    void dezoom(float by = 0.1f);

    void moveHorizontal(float distance);
    void moveVertical(float distance);

    glm::mat4 getViewMatrix() const;
    glm::vec3 getEye() const;

    void reset(const glm::vec3& center, float radius, float azimuthAngle, float polarAngle);

    void setCenter(const glm::vec3& center);
    void setRadius(float radius);
    void setAzimuthAngle(float azimuthAngle);
    void setPolarAngle(float polarAngle);

    const glm::vec3& center() const;
    float radius() const;
    float azimuthAngle() const;
    float polarAngle() const;

private:
    glm::vec3 getNormalizedViewVector() const;

private:
    glm::vec3 m_center; // Center of the orbit camera sphere (the point upon which the camera looks)
    glm::vec3 m_upVector; // Up vector of the camera
    float m_radius; // Radius of the orbit camera sphere
    float m_minRadius; // Minimal radius of the orbit camera sphere (cannot fall below this value)
    float m_maxRadius; // Maximal radius of the orbit camera sphere (cannot go beyond this value)
    float m_azimuthAngle; // Azimuth angle on the orbit camera sphere
    float m_polarAngle; // Polar angle on the orbit camera sphere
};

} // Core

#endif //AUTOFRAC_CAMERA_H
