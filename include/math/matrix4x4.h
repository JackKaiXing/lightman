#ifndef _LIGHTMAN_MATRIX4X4_H
#define _LIGHTMAN_MATRIX4X4_H

#include <cstddef>
#include <cstring>
#include <cmath>
#include <iostream>

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
            TMatrix4X4 Inverse();
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

        template<typename T>
        TMatrix4X4<T> TMatrix4X4<T>::Inverse()
        {
            int indxc[4], indxr[4];
            int ipiv[4] = {0, 0, 0, 0};
            float minv[4][4];
            memcpy(minv, m_value, 4 * 4 * sizeof (float));
            for (int i = 0; i < 4; ++i) {
                int irow = -1, icol = -1;
                float big = 0.;
                // Choose pivot
                for (int j = 0; j < 4; ++j) {
                    if (ipiv[j] != 1) {
                        for (int k = 0; k < 4; ++k) {
                            if (ipiv[k] == 0) {
                                if (fabsf(minv[j][k]) >= big) {
                                    big = fabsf(minv[j][k]);
                                    irow = j;
                                    icol = k;
                                }
                            } else if (ipiv[k] > 1)
                            {
                                std::cout << "Singular matrix in MatrixInvert" << std::endl;
                                assert(0);
                            }   
                        }
                    }
                }
                ++ipiv[icol];
                // Swap rows _irow_ and _icol_ for pivot
                if (irow != icol) {
                    for (int k = 0; k < 4; ++k)
                    {
                        const T temp = minv[irow][k];
                        minv[irow][k] = minv[icol][k];
                        minv[icol][k] = temp;
                    }
                }
                indxr[i] = irow;
                indxc[i] = icol;
                if (minv[icol][icol] == 0.f)
                {
                    std::cout << "Singular matrix in MatrixInvert" << std::endl;
                    assert(0);
                }
                // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
                float pivinv = 1.f / minv[icol][icol];
                minv[icol][icol] = 1.f;
                for (int j = 0; j < 4; ++j)
                    minv[icol][j] *= pivinv;
                // Subtract this row from others to zero out their columns
                for (int j = 0; j < 4; ++j) {
                    if (j != icol) {
                        float save = minv[j][icol];
                        minv[j][icol] = 0;
                        for (int k = 0; k < 4; ++k)
                            minv[j][k] -= minv[icol][k] * save;
                    }
                }
            }
            // Swap columns to reflect permutation
            for (int j = 3; j >= 0; --j) {
                if (indxr[j] != indxc[j]) {
                    for (int k = 0; k < 4; ++k)
                    {
                        const T temp = minv[k][indxr[j]];
                        minv[k][indxr[j]] = minv[k][indxc[j]];
                        minv[k][indxc[j]] = temp;
                    }
                }
            }

            return TMatrix4X4<T>(minv);
        }
        // ----------------------------------------------------------------------------
        using Matrix4X4 = TMatrix4X4<float>;
        
    } // namespace math
    
} // namespace lightman
#endif  // _LIGHTMAN_MATRIX4X4_H