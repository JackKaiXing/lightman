#ifndef _LIGHTMAN_VIEW_H
#define _LIGHTMAN_VIEW_H

#include "scene/scene.h"

using namespace lightman::scene;
namespace lightman
{
    namespace view
    {
        class View
        {
        public:
            View();
            virtual ~View();
        private:
            Scene * m_scene;
        };
    }
}
#endif // _LIGHTMAN_VIEW_H