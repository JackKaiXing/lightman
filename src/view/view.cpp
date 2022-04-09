
#include "view/view.h"

namespace lightman
{
    View::View()
    {
        m_scene = nullptr;
    }
    View::~View()
    {
        if(m_scene)
            m_scene->ReleaseRef();
        if(m_camera)
            m_camera->ReleaseRef();
    }
    void View::SetScene(Scene * targetScene)
    {
        if (m_scene)
        {
            m_scene->ReleaseRef();
        }
        if (targetScene)
        {
            targetScene->IncreaseRef();
        }
        m_scene = targetScene;
    }
    void View::SetCamera(Camera * camera)
    {
        if(m_camera)
            m_camera->ReleaseRef();
        if(camera)
            camera->IncreaseRef();
        m_camera = camera;    
    }
    Scene* View::GetScene()
    {
        return m_scene;
    }
}
