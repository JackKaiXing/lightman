#ifndef _LIGHTMAN_IMAGEMAPMANAGER_H
#define _LIGHTMAN_IMAGEMAPMANAGER_H

#include <unordered_map>
#include <string>

#include "texture/imagemap.h"

namespace lightman
{
    class ImagemapManager
    {
    public:
        static ImagemapManager* GetInstance();
        static void DetoryInstance();
        Imagemap* GetImagemap(const std::string& name);
        Imagemap* LoadImagemap(const std::string& path, const std::string& fileName);

    protected:
        ImagemapManager(/* args */);
        ~ImagemapManager();
    private:
        std::unordered_map<std::string, Imagemap*> m_imagemaps;
    };
} // namespace lightman

#endif  //_LIGHTMAN_IMAGEMAPMANAGER_H