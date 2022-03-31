
#include "view/view.h"

namespace lightman
{
    namespace view
    {
        View::View()
        {
            m_scene = nullptr;
        }
        View::~View()
        {
            if(m_scene)
                m_scene->Release();
        }
        void View::SetScene(Scene * targetScene)
        {
            if (m_scene)
            {
                m_scene->Release();
            }
            if (targetScene)
            {
                targetScene->Increase();
            }
            
            m_scene = targetScene;
        }
    }
}
