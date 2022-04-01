#ifndef _LIGHTMAN_CAMERA_H
#define _LIGHTMAN_CAMERA_H

#include "utils/ref.h"
#include "math/matrix4x4.h"

using namespace lightman::utils;
using namespace lightman::math;

namespace lightman
{
    namespace camera
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
        };
    }
}
#endif // _LIGHTMAN_CAMERA_H