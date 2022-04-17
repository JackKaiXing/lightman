#ifndef _LIGHTMAN_MATHUTILS_H
#define _LIGHTMAN_MATHUTILS_H

#include <cmath>

namespace lightman
{
    namespace math
    {
        // ----------------------------------------------------------------------------
        inline void V3ASubB(const float* a, const float* b, float* c)
        {
            c[0] = a[0] - b[0];
            c[1] = a[1] - b[1];
            c[2] = a[2] - b[2];
        }
        inline float V3SquaredLength(const float* a)
        {
            return a[0]*a[0] + a[1]*a[1] + a[2]*a[2];
        }
        inline float V3Length(const float* a)
        {
            return sqrtf(V3SquaredLength(a));
        }
        inline void V3Normalize(float* a)
        {
            float length = V3Length(a);
            a[0] /= length;
            a[1] /= length;
            a[2] /= length;
        }
        inline void V3Cross(const float* a, const float* b, float* c)
        {
            c[0] = a[1]*b[2] - a[2]*b[1];
            c[1] = a[2]*b[0] - a[0]*b[2];
            c[2] = a[0]*b[1] - a[1]*b[0];
        }
    } // namespace math
    
} // namespace lightman
#endif  // _LIGHTMAN_MATHUTILS_H