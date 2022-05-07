#ifndef _LIGHTMAN_VECTOR_H
#define _LIGHTMAN_VECTOR_H

#include <cstddef>

namespace lightman
{
    namespace math
    {
        template<typename T>
        class TVector4
        {
        public:
            static constexpr size_t SIZE = 4;
            TVector4(/* args */)
            {
                for (size_t i = 0; i < SIZE; i++)
                {
                    v[i] = 0.0;
                }
            };
            TVector4(T v0, T v1, T v2, T v3)
            {
                v[0] = v0;
                v[1] = v1;
                v[2] = v2;
                v[3] = v3;
            }
            ~TVector4(){};
            T v[SIZE];
        };

        template<typename T>
        class TVector3
        {
        public:
            static constexpr size_t SIZE = 3;
            TVector3(/* args */)
            {
                for (size_t i = 0; i < SIZE; i++)
                {
                    v[i] = 0.0;
                }
            };
            ~TVector3(){};
            T v[SIZE];
        };
        // ----------------------------------------------------------------------------
        using Vector4 = TVector4<float>;
    }
}
#endif //  _LIGHTMAN_VECTOR_H
