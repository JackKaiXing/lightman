#ifndef _LIGHTMAN_TRANSFORM_H
#define _LIGHTMAN_TRANSFORM_H

#include "math/matrix4x4.h"

namespace lightman
{
    namespace math
    {
        // ----------------------------------------------------------------------------
        class Transform
        {
        public:
            void setMatrix4(const Matrix4X4& mat);
        private:
            Matrix4X4 m_Mat, m_invMat;
        };
        // ----------------------------------------------------------------------------
        
    } // namespace math
    
} // namespace lightman
#endif  // _LIGHTMAN_TRANSFORM_H