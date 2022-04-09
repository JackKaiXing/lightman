#ifndef _LIGHTMAN_MESHMANAGER_H
#define _LIGHTMAN_MESHMANAGER_H

#include <unordered_map>
#include <string>

#include "geometry/mesh.h"

namespace lightman
{
    class Mesh;
    class MeshManager
    {
    public:
        static MeshManager* GetInstance();
        static void DetoryInstance();
    protected:
        MeshManager(/* args */);
        ~MeshManager();
    private:
        std::unordered_map<std::string, Mesh*> m_meshes;
    };
} // namespace lightman

#endif  //_LIGHTMAN_MESHMANAGER_H