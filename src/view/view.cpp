
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
            m_scene->Release();
        if(m_camera)
            m_camera->Release();
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
    void View::SetCamera(Camera * camera)
    {
        if(m_camera)
            m_camera->Release();
        if(camera)
            camera->Increase();
        m_camera = camera;    
    }
    Scene* View::GetScene()
    {
        return m_scene;
    }
}
