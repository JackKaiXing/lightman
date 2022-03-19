#ifndef _LIGHTMAN_QUATERNION_H
#define _LIGHTMAN_QUATERNION_H

#include "geometry/vector.h"

using namespace lightman::geometry;

namespace lightman
{
    namespace math
    {
        // ----------------------------------------------------------------------------
        class Quaternion
        {
        public:
            Vector m_vector;
            float m_radians;
        };
    } // namespace math
    
} // namespace lightman
#endif  // _LIGHTMAN_QUATERNION_H