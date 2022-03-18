#ifndef _LIGHTMAN_UV_H
#define _LIGHTMAN_UV_H

namespace lightman
{
    namespace geometry
    {
        // ----------------------------------------------------------------------------
        class UV
        {
        public:
            float u,v;
            UV(){ u = v = 0; };
            UV(float _u, float _v) : u(_u), v(_v) {};
        };
    } // namespace geometry
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_UV_H