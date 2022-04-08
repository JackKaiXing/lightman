#ifndef _LIGHTMAN_SCENE_H
#define _LIGHTMAN_SCENE_H

#include <string>
#include <unordered_map>

#include "geometry/trianglemesh.h"
#include "objects/instancedtrianglemesh.h"
#include "materials/material.h"

#include "utils/ref.h"

using namespace std;
using namespace lightman::objects;
using namespace lightman::materials;
using namespace lightman::utils;

namespace lightman
{
    // ----------------------------------------------------------------------------
    namespace scene
    {
        class Scene : public Ref
        {
        public:
            void Parse(const string& file);
            void PreRender();
        private:
            // TODO: Managers for those resources
            std::unordered_map<string, InstancedTriangleMesh*> m_iMeshes;
            std::unordered_map<string, TriangleMesh*> m_meshes;
            std::unordered_map<string, Material*> m_mats;
        };
    } // namespace scene
} // namespace lightman

#endif // #ifndef _LIGHTMAN_SCENE_H