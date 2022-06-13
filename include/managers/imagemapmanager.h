#ifndef _LIGHTMAN_IMAGEMAPMANAGER_H
#define _LIGHTMAN_IMAGEMAPMANAGER_H

#include <unordered_map>
#include <string>

#include "materialnode/imagemap.h"
#include "backend/driverbase.h"

namespace lightman
{
    class ImagemapManager
    {
        struct ImagemapData
        {
            ImagemapData(Imagemap* m, backend::HwTexture* t)
            {
                img = m;
                texture = t;
            };
            ~ImagemapData()
            {
                if (img)
                    delete img;
                 if (texture)
                    delete texture;  // TODO DESTORY BY ENGINE DRIVER 
            };
            Imagemap* img;
            backend::HwTexture* texture;
        };
        
    public:
        static ImagemapManager* GetInstance();
        static void DetoryInstance();
        Imagemap* GetImagemap(const std::string& name);
        Imagemap* LoadImagemap(const std::string& path, const std::string& fileName);
        backend::HwTexture* GetHwTexture(const std::string& name);

    protected:
        ImagemapManager(/* args */);
        ~ImagemapManager();
    private:
        std::unordered_map<std::string, ImagemapData*> m_imagemaps;
    };
} // namespace lightman

#endif  //_LIGHTMAN_IMAGEMAPMANAGER_H
