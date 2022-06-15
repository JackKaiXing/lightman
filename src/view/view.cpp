
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
            RELEASEORDELETE(m_scene);
        if(m_camera)
            RELEASEORDELETE(m_camera);
    }
    void View::SetScene(Scene * targetScene)
    {
        if (m_scene)
            RELEASEORDELETE(m_scene);
        if (targetScene)
        {
            targetScene->IncreaseRef();
        }
        m_scene = targetScene;
    }
    void View::SetCamera(Camera * camera)
    {
        if(m_camera)
            RELEASEORDELETE(m_camera);
        if(camera)
            camera->IncreaseRef();
        m_camera = camera;    
    }
    Scene* View::GetScene()
    {
        return m_scene;
    }
    Camera* View::getCamera()
    {
        return m_camera;
    }
}
