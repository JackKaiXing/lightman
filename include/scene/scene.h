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
            void Parse(const string& file);
        friend class GPURenderer;
        protected:
            std::unordered_map<string, InstancedTriangleMesh*> GetInstanceMeshes();
        private:
            std::unordered_map<string, InstancedTriangleMesh*> m_iMeshes;
            std::unordered_map<string, string> m_iMaterials;
    };
} // namespace lightman

#endif // #ifndef _LIGHTMAN_SCENE_H