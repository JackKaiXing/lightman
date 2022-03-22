#ifndef _LIGHTMAN_SCENE_H
#define _LIGHTMAN_SCENE_H

#include <string>
#include <unordered_map>

#include "geometry/trianglemesh.h"
#include "objects/instancedtrianglemesh.h"

using namespace std;
using namespace lightman::objects;

namespace lightman
{
    // ----------------------------------------------------------------------------
    namespace scene
    {
        class Scene
        {
        public:
            void Parse(const string& file);
        private:
            std::unordered_map<string, InstancedTriangleMesh*> m_iMeshes;
            std::unordered_map<string, TriangleMesh*> m_meshes;
        };
    } // namespace scene
} // namespace lightman

#endif // #ifndef _LIGHTMAN_SCENE_H