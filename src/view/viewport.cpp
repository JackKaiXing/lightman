
#include "view/viewport.h"
#include <assert.h>

namespace lightman
{
    Viewport::Viewport()
    {

    }
    Viewport::Viewport(int32_t left, int32_t bottom, uint32_t width, uint32_t height)
    {
        assert(!(width < 0 || height< 0));

        m_left = left;
        m_bottom = bottom;
        m_width = width;
        m_height = height;
    }
    Viewport::~Viewport()
    {

    }
}