#ifndef _LIGHTMAN_MESHMANAGER_H
#define _LIGHTMAN_MESHMANAGER_H

#include <unordered_map>
#include <string>
#include <vector>

#include "geometry/mesh.h"

using namespace lightman::geometry;

namespace lightman
{
    class MeshManager
    {
    public:
        static MeshManager* GetInstance();
        static void DetoryInstance();
        bool HasMesh(std::string name);
        void CreateTriMesh(std::string name,
            std::vector<unsigned int>& triIndexs, std::vector<float>& points,
            std::vector<float>& normals, std::vector<float>& uvs);
    protected:
        MeshManager(/* args */);
        ~MeshManager();
    private:
        std::unordered_map<std::string, Mesh*> m_meshes;
    };
} // namespace lightman

#endif  //_LIGHTMAN_MESHMANAGER_H