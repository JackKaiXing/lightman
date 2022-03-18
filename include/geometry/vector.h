#ifndef _LIGHTMAN_VECTOR_H
#define _LIGHTMAN_VECTOR_H

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        class Vector
        {   
        public:
            float x,y,z;
            Vector(){ x = y = z = 0; };
            Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
        };
    } // namespace math
} // namespace lightman
#endif //_LIGHTMAN_VECTOR_H