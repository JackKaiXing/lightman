#ifndef _LIGHTMAN_MATRIX4X4_H
#define _LIGHTMAN_MATRIX4X4_H

#include <cstddef>
#include <cstring>
#include <cmath>
#include <iostream>
#include <assert.h>

#include "math/mathutils.h"
#include "math/vector.h"

namespace lightman
{
    namespace math
    {
        // ----------------------------------------------------------------------------
        template<typename T>
        class TMatrix4X4
        {
        public:
            // row first
            static constexpr size_t SIZE = 4;
            TMatrix4X4();
            TMatrix4X4(const T mat[SIZE][SIZE]);
            TMatrix4X4 Inverse();
            void Transpose();
            bool SwapsHandedness();
            static TMatrix4X4 LookAt(T eye[3], T target[3], T up[3]);
            static TMatrix4X4 frustum(T left, T right, T bottom, T top, T near, T far);
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
        template<typename T>
        TMatrix4X4<T> TMatrix4X4<T>::frustum(T left, T right, T bottom, T top, T near, T far)
        {
            // http://learnwebgl.brown37.net/08_projections/projections_perspective.html
            /*
             * P =  2N/r-l    0      r+l/r-l        0
             *       0      2N/t-b   t+b/t-b        0
             *       0        0      F+N/N-F   2*F*N/N-F
             *       0        0        -1           0
             */
            // http://learnwebgl.brown37.net/08_projections/projections_perspective.html

            TMatrix4X4<T> m;
            m.m_value[0][0] = (2 * near) / (right - left);
            m.m_value[0][2] = (right + left) / (right - left);
            
            m.m_value[1][1] = (2 * near) / (top - bottom);
            m.m_value[1][2] = (top + bottom) / (top - bottom);
            
            m.m_value[2][2] = -(far + near) / (far - near);
            m.m_value[2][3] = -(2 * far * near) / (far - near);
            
            m.m_value[3][2] = -1;
            m.m_value[3][3] = 0;
                        
            return m;
        }
        template<typename T>
        TMatrix4X4<T> TMatrix4X4<T>::LookAt(T eye[3], T target[3], T up[3])
        {
            // camera to wolrd, row first
            TMatrix4X4<T> cameToWorld;
            cameToWorld.m_value[0][3] = eye[0];
            cameToWorld.m_value[1][3] = eye[1];
            cameToWorld.m_value[2][3] = eye[2];
            cameToWorld.m_value[3][3] = 1;
            float dir[3] = {};
            V3ASubB(target, eye, dir);
            V3Normalize(dir);
            float right[3] = {};
            V3Cross(dir, up, right);
            V3Normalize(right);
            float newUp[3] = {};
            V3Cross(right, dir, newUp);
            cameToWorld.m_value[0][0] = right[0];
            cameToWorld.m_value[1][0] = right[1];
            cameToWorld.m_value[2][0] = right[2];
            cameToWorld.m_value[3][0] = 0.;
            cameToWorld.m_value[0][1] = newUp[0];
            cameToWorld.m_value[1][1] = newUp[1];
            cameToWorld.m_value[2][1] = newUp[2];
            cameToWorld.m_value[3][1] = 0.;
            cameToWorld.m_value[0][2] = -dir[0];
            cameToWorld.m_value[1][2] = -dir[1];
            cameToWorld.m_value[2][2] = -dir[2];
            cameToWorld.m_value[3][2] = 0.;
            
            // camToWorld, https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function
            // worldToCamera, same as gllookat, https://twodee.org/blog/17560
            // the camera should look into the negative-Z axis, http://www.songho.ca/opengl/gl_projectionmatrix.html
            return cameToWorld;
        }
        template<typename T>
        void TMatrix4X4<T>::Transpose()
        {
            float temp = 0.0;
            for (int i = 0; i < TMatrix4X4<T>::SIZE; i++)
            {
                for (int j = i+1; j < TMatrix4X4<T>::SIZE; j++)
                {
                    temp = m_value[i][j];
                    m_value[i][j] = m_value[j][i];
                    m_value[j][i] = temp;
                }
            }
        }
        template<typename T>
        bool TMatrix4X4<T>::SwapsHandedness()
        {
            // https://www.euclideanspace.com/maths/geometry/elements/determinant/index.htm
            // https://www.photomodeler.com/downloads/OnlineHelp/index.html#!righthandedcoordinatesystem.htm
            const float det = (
                (m_value[0][0] * (m_value[1][1] * m_value[2][2] - m_value[1][2] * m_value[2][1])) -
                (m_value[0][1] * (m_value[1][0] * m_value[2][2] - m_value[1][2] * m_value[2][0])) +
                (m_value[0][2] * (m_value[1][0] * m_value[2][1] - m_value[1][1] * m_value[2][0])));
            return det < 0.f;
        }
        // Should Not be Member Function, https://stackoverflow.com/questions/13554320/overloaded-operator-must-be-a-unary-or-binary-operator-error
        template<typename T>
        TMatrix4X4<T> operator*(const TMatrix4X4<T>& lhs, const TMatrix4X4<T>& rhs)
        {
            TMatrix4X4<T> result;

            // Normal Matrix Multiply
            for (int i = 0; i < TMatrix4X4<T>::SIZE; i++)
            {
                for (int j = 0; j < TMatrix4X4<T>::SIZE; j++)
                {
                    result.m_value[i][j] = 0;
                    for (int k = 0; k < TMatrix4X4<T>::SIZE; k++)
                    {
                        result.m_value[i][j] += lhs.m_value[i][k] * rhs.m_value[k][j];
                    }
                }
            }
            // FIXME better Matrix Multiply Performance
            // https://github.com/rangelak/Strassen-Matrix-Multiplication/blob/master/strassen.cpp

            return result;
        }
        template<typename T>
        Vector4 operator*(const TMatrix4X4<T>& lhs, const Vector4& rhs)
        {
            Vector4 result;

            for (size_t i = 0; i <TMatrix4X4<T>::SIZE; i++)
            {
                result.v[i] = 0.0;
                for (size_t j = 0; j < TMatrix4X4<T>::SIZE; j++)
                {
                    result.v[i] += lhs.m_value[i][j] * rhs.v[j];
                }
            }
        
            return result;
        }
        // ----------------------------------------------------------------------------
        using Matrix4X4 = TMatrix4X4<float>;
        
    } // namespace math
    
} // namespace lightman
#endif  // _LIGHTMAN_MATRIX4X4_H
