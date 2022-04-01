#ifndef _LIGHTMAN_VIEW_H
#define _LIGHTMAN_VIEW_H

#include "scene/scene.h"
#include "view/viewport.h"
#include "camera/camera.h"

using namespace lightman::scene;
using namespace lightman::camera;

namespace lightman
{
    namespace view
    {
        class View
        {
        public:
            View();
            virtual ~View();
            void SetScene(Scene * targetScene);
            void SetCamera(Camera * camera);
        private:
            Scene * m_scene = nullptr;
            Viewport m_viewport;
            Camera * m_camera = nullptr;
        };
    }
}
#endif // _LIGHTMAN_VIEW_H
