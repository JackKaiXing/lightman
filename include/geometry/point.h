#ifndef _LIGHTMAN_POINT_H
#define _LIGHTMAN_POINT_H

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        class Point
        {
        public:
            float x,y,z;
            Point(){ x = y = z = 0; };
            Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_POINT_H