#ifndef _LIGHTMAN_VIEW_H
#define _LIGHTMAN_VIEW_H

#include "scene/scene.h"
#include "view/viewport.h"
#include "camera/camera.h"

namespace lightman
{
    class View
    {
    public:
        View();
        virtual ~View();
        void SetScene(Scene * targetScene);
        void SetCamera(Camera * camera);
        Scene* GetScene();
    private:
        Scene * m_scene = nullptr;
        Viewport m_viewport;
        Camera * m_camera = nullptr;
    };
}
#endif // _LIGHTMAN_VIEW_H
