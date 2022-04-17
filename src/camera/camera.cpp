
#include "camera/camera.h"

#include <iterator>
#include <algorithm>
#include "math/matrix4x4.h"
#include "math/mathutils.h"

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

        float m[4][4];
        m[0][3] = eye[0];
        m[1][3] = eye[1];
        m[2][3] = eye[2];
        m[3][3] = 1;
        float dir[3] = {};
        V3ASubB(m_target, m_eye, dir);
        V3Normalize(dir);
        float right[3] = {};
        V3Cross(dir, up, right);
        V3Normalize(right);
        float newUp[3] = {};
        V3Cross(right, dir, newUp);
        m[0][0] = right[0];
        m[1][0] = right[1];
        m[2][0] = right[2];
        m[3][0] = 0.;
        m[0][1] = newUp[0];
        m[1][1] = newUp[1];
        m[2][1] = newUp[2];
        m[3][1] = 0.;
        m[0][2] = dir[0];
        m[1][2] = dir[1];
        m[2][2] = dir[2];
        m[3][2] = 0.;
        // https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function
        m_cameraToWorld = Matrix4X4(m); 
        // same as gllookat, https://twodee.org/blog/17560
        m_worldToCamera = m_cameraToWorld.Inverse(); 
    }
}
