#ifndef _LIGHTMAN_BBOX_H
#define _LIGHTMAN_BBOX_H

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        class BBox
        {
        public:
            float m_pMin[3], m_pMax[3];
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_BBOX_H