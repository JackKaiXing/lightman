#ifndef _LIGHTMAN_BBOX_H
#define _LIGHTMAN_BBOX_H

#include "geometry/point.h"

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        class BBox
        {
        public:
            Point m_PMin, m_PMax;
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_BBOX_H