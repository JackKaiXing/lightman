
#include "camera/camera.h"

#include <assert.h>
#include <iterator>
#include <algorithm>
#include "math/matrix4x4.h"

using namespace lightman::utils;
using namespace lightman::math;

namespace lightman
{
    Camera::Camera()
    {

    }
    Camera::~Camera()
    {

    }
    void Camera::LookAt(float eye[3], float target[3], float up[3])
    {
        std::copy(eye, eye+3, m_eye);
        std::copy(target, target+3, m_target);
        std::copy(up, up+3, m_up);
        
        m_cameraToWorld = Matrix4X4::LookAt(m_eye, m_target, m_up); 
        m_worldToCamera = m_cameraToWorld.Inverse(); 
        m_worldToSreen = m_cameraToScreen * m_worldToCamera;
        isCameraMovingConsumed = false;
    }
    void Camera::setProjection(float fovDegree, float near, float far, float aspect, Camera::FovDirection direction)
    {
        float w, h;
        double s = std::tan(fovDegree * math::DEG_TO_RAD / 2.0) * near;
        if (direction == FovDirection::VERTICAL)
        {
            w = s * aspect;
            h = s;
        } else {
            w = s;
            h = s / aspect;
        }
        Camera::setProjection(-w, w, -h, h, near, far);
    }
    void Camera::setProjection(float left, float right, float bottom, float up, float near, float far)
    {
        assert( !(
                  left == right ||
                  bottom == up ||
                  (GetCameraType() == CameraType::PERSPECTIVE && (near < 0 || far <= near))
                  )
        );

        switch (GetCameraType())
        {
        case CameraType::PERSPECTIVE:
            m_cameraToScreen = Matrix4X4::frustum(left, right, bottom, up, near, far);
            break;
        case CameraType::ORTHO:
            /* code */
            break;
        }

        m_near = near;
        m_far = far;
        
        m_worldToSreen = m_cameraToScreen * m_worldToCamera;
        isCameraMovingConsumed = false;
    }
    Matrix4X4 Camera::GetViewMatrix()
    {
        return m_worldToCamera;
    }
    Matrix4X4 Camera::GetProjectionMatrix()
    {
        return m_cameraToScreen;
    }
    Matrix4X4 Camera::GetProjectionViewMatrix()
    {
        return m_worldToSreen;
    }

    Vector3 Camera::GetWorldPosition()
    {
        Vector3 result;
        result.v[0] = m_cameraToWorld.m_value[0][3];
        result.v[1] = m_cameraToWorld.m_value[1][3];
        result.v[2] = m_cameraToWorld.m_value[2][3];

        return result;
    }
}
