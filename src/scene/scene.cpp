
#include "scene/scene.h"

namespace lightman
{
namespace scene
{
    void Scene::PreRender()
    {
        std::unordered_map<string, InstancedTriangleMesh*>::iterator iter = m_iMeshes.begin();;
        while( iter !=m_iMeshes.end() )
        {
            iter->second->preRender();
            iter++;
        }
    }
// ----------------------------------------------------------------------------
}
}