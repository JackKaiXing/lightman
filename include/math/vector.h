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
            TVector3(T v0, T v1, T v2)
            {
                v[0] = v0;
                v[1] = v1;
                v[2] = v2;
            }
            static TVector3 Cross(const TVector3& a, const TVector3& b);
            ~TVector3(){};
            T v[SIZE];
        };

        template<typename T>
        TVector3<T> TVector3<T>::Cross(const TVector3<T> & a, const TVector3<T> & b)
        {
            TVector3<T> result;

            result.v[0] = a.v[1]*b.v[2] - a.v[2]*b.v[1];
            result.v[1] = a.v[2]*b.v[0] - a.v[0]*b.v[2];
            result.v[2] = a.v[0]*b.v[1] - a.v[1]*b.v[0];
            
            return result;
        }

        template<typename T>
        TVector3<T> operator-(const TVector3<T>& lhs, const TVector3<T>& rhs)
        {
            TVector3<T> result;
            int j = 0;
            
            result.v[j] = lhs.v[j] - rhs.v[j]; j++;
            result.v[j] = lhs.v[j] - rhs.v[j]; j++;
            result.v[j] = lhs.v[j] - rhs.v[j];
            
            return result;
        }
        // ----------------------------------------------------------------------------
        using Vector4 = TVector4<float>;
        using Vector3 = TVector3<float>;
    }
}
#endif //  _LIGHTMAN_VECTOR_H
