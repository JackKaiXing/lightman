#ifndef _LIGHTMAN_MATRIX4X4_H
#define _LIGHTMAN_MATRIX4X4_H

namespace lightman
{
    namespace math
    {
        // ----------------------------------------------------------------------------
        template<typename T>
        class TMatrix4X4
        {
            static constexpr size_t SIZE = 4;
        public:
            T m[SIZE][SIZE];
        };
        // ----------------------------------------------------------------------------
        using Matrix4X4 = TMatrix4X4<float>;
    } // namespace math
    
} // namespace lightman
#endif  // _LIGHTMAN_MATRIX4X4_H