#ifndef _LIGHTMAN_QUATERNION_H
#define _LIGHTMAN_QUATERNION_H

namespace lightman
{
    namespace math
    {
        // ----------------------------------------------------------------------------
        class Quaternion
        {
        public:
            float m_vector[3];
            float m_radians;
        };
    } // namespace math
    
} // namespace lightman
#endif  // _LIGHTMAN_QUATERNION_H