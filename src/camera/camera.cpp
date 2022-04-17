
#include "camera/camera.h"

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
    }
}
