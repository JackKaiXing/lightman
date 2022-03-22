#ifndef _LIGHTMAN_TRIANGLE_H
#define _LIGHTMAN_TRIANGLE_H

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        class Triangle
        {
        public:
            Triangle(unsigned int v0, unsigned int v1, unsigned int v2)
            {
                 m_vertex[0] = v0;
                 m_vertex[1] = v1;
                 m_vertex[2] = v2;
            }
        private:
            unsigned int m_vertex[3];
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_TRIANGLE_H