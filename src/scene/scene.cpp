
#include "scene/scene.h"

namespace lightman
{
    std::unordered_map<string, InstancedTriangleMesh*> Scene::GetInstanceMeshes()
    {
        return m_iMeshes;
    }
}