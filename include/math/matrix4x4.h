#ifndef _LIGHTMAN_MATRIX4X4_H
#define _LIGHTMAN_MATRIX4X4_H

#include <cstddef>
#include <cstring>

namespace lightman
{
    namespace math
    {
        // ----------------------------------------------------------------------------
        template<typename T>
        class TMatrix4X4
        {
        public:
            static constexpr size_t SIZE = 4;
            TMatrix4X4();
            TMatrix4X4(const T mat[SIZE][SIZE]);
        private:
            T m_value[SIZE][SIZE];
        };

        template<typename T>
        TMatrix4X4<T>::TMatrix4X4()
        {
            for (int i = 0; i < TMatrix4X4::SIZE; ++i)
            {
                for (int j = 0; j < TMatrix4X4::SIZE; ++j)
                {
                    if (i == j)
					    m_value[i][j] = (T)1.0f;
				    else
					    m_value[i][j] = (T)0.0f;
                }
            }
        }

        template<typename T>
        TMatrix4X4<T>::TMatrix4X4(const T mat[TMatrix4X4::SIZE][TMatrix4X4::SIZE])
        {
            memcpy(m_value, mat, TMatrix4X4::SIZE * TMatrix4X4::SIZE * sizeof (T));
        }
        // ----------------------------------------------------------------------------
        using Matrix4X4 = TMatrix4X4<float>;
        
    } // namespace math
    
} // namespace lightman
#endif  // _LIGHTMAN_MATRIX4X4_H