#ifndef _LIGHTMAN_IMAGEMAPMANAGER_H
#define _LIGHTMAN_IMAGEMAPMANAGER_H

#include <unordered_map>
#include <string>

namespace lightman
{
    class ImagemapManager
    {
    public:
        static ImagemapManager* GetInstance();
        static void DetoryInstance();
    protected:
        ImagemapManager(/* args */);
        ~ImagemapManager();
    private:
        std::unordered_map<std::string, int> m_imagemaps;
    };
} // namespace lightman

#endif  //_LIGHTMAN_IMAGEMAPMANAGER_H