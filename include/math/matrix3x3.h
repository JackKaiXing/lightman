#ifndef _LIGHTMAN_MATRIX3X3_H
#define _LIGHTMAN_MATRIX3X3_H

namespace lightman
{
    namespace math
    {
        // ----------------------------------------------------------------------------
        template<typename T>
        class TMatrix3X3
        {
            static constexpr size_t SIZE = 3;
        public:
            T m[SIZE][SIZE];
        };
        // ----------------------------------------------------------------------------
        using Matrix3X3 = TMatrix3X3<float>;
    } // namespace math
    
} // namespace lightman
#endif  // _LIGHTMAN_MATRIX3X3_H