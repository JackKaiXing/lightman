#ifndef _LIGHTMAN_COLOR_H
#define _LIGHTMAN_COLOR_H

namespace lightman
{
    // ----------------------------------------------------------------------------
    class Color
    {
    public:
        Color() {m_c[0] = 0.0; m_c[1] = 0.0; m_c[2] = 0.0;};
        Color(float c) {m_c[0] = c; m_c[1] = c; m_c[2] = c;};
        Color(float c0, float c1, float c2) {m_c[0] = c0; m_c[1] = c1; m_c[2] = c2;};
        float GetR() {return m_c[0];};
        float GetG() {return m_c[1];};
        float GetB() {return m_c[2];};
    private:
        float m_c[3];
    };
    
} // namespace lightman

#endif // #ifndef _LIGHTMAN_COLOR_H