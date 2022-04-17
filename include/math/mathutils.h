#ifndef _LIGHTMAN_MATHUTILS_H
#define _LIGHTMAN_MATHUTILS_H

#include <cmath>

namespace lightman
{
    namespace math
    {
        // ----------------------------------------------------------------------------
        constexpr const double L_PI             = 3.14159265358979323846264338327950288;
        constexpr const double DEG_TO_RAD       = L_PI/ 180.0;
        constexpr const double RAD_TO_DEG       = 180.0 / L_PI;

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