#ifndef _LIGHTMAN_RAY_H
#define _LIGHTMAN_RAY_H

#include "geometry/point.h"
#include "geometry/vector.h"

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        class Ray
        {
        public:
            Point m_point;
            Vector m_vector;
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_RAY_H