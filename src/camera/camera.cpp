
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

        
    }
}
