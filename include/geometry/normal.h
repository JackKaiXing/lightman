#ifndef _LIGHTMAN_NORMAL_H
#define _LIGHTMAN_NORMAL_H

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        class Normal
        {
        public:
            float x,y,z;
            Normal(){ x = y = z = 0; };
            Normal(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_NORMAL_H