
#include "renderer/renderer.h"

namespace lightman
{
    namespace renderer
    {
        //----------------------------------------------------------------------------
        Renderer::Renderer(Engine * engine)
        {
            m_engine = engine;
        }
        Renderer::~Renderer()
        {
            m_engine = nullptr;
        }
        //----------------------------------------------------------------------------
        GPURenderer::GPURenderer(Engine * engine) : Renderer(engine)
        {

        }
    }
}