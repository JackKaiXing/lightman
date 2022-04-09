#ifndef _LIGHTMAN_SCENE_H
#define _LIGHTMAN_SCENE_H

#include <string>
#include <unordered_map>

#include "geometry/trianglemesh.h"
#include "objects/instancedtrianglemesh.h"
#include "materials/material.h"

#include "utils/ref.h"

using namespace std;
using namespace lightman::utils;

namespace lightman
{
    // ----------------------------------------------------------------------------
    class Scene : public Ref
        {
        public:
            void Parse(const string& file);
        friend class View;
        protected:
            void PreRender();
        private:
            // TODO: Managers for those resources
            std::unordered_map<string, InstancedTriangleMesh*> m_iMeshes;
            std::unordered_map<string, TriangleMesh*> m_meshes;
            std::unordered_map<string, Material*> m_mats;
    };
} // namespace lightman

#endif // #ifndef _LIGHTMAN_SCENE_H