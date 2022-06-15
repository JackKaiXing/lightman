#ifndef _LIGHTMAN_SCENE_H
#define _LIGHTMAN_SCENE_H

#include <string>
#include <unordered_map>

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
            Scene();
            ~Scene();
            InstancedTriangleMesh* GetMesh(const std::string& meshName);
            InstancedTriangleMesh* AddGetMesh(const std::string& meshName);
        friend class GPURenderer;
        protected:
            std::unordered_map<string, InstancedTriangleMesh*> GetInstanceMeshes();
        private:
            std::unordered_map<string, InstancedTriangleMesh*> m_iMeshes;
    };
} // namespace lightman

#endif // #ifndef _LIGHTMAN_SCENE_H
