#ifndef _LIGHTMAN_CAMERA_H
#define _LIGHTMAN_CAMERA_H

#include "utils/ref.h"
#include "math/matrix4x4.h"
#include "geometry/point.h"
#include "geometry/vector.h"

using namespace lightman::utils;
using namespace lightman::math;
using namespace lightman::geometry;

namespace lightman
{
    class Camera : public Ref
    {
    public:
        Camera();
        virtual ~Camera();
    private:
        float m_near{};
        float m_far{};
        Matrix4X4 m_projection;
        Point m_eye;
        Point m_center;
        Vector m_up;
    };
}
#endif // _LIGHTMAN_CAMERA_H