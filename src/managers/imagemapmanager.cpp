
#include "managers/imagemapmanager.h"
#include "stb/stb_image.h"

namespace lightman
{
    ImagemapManager* g_imManager = nullptr;
    ImagemapManager* ImagemapManager::GetInstance()
    {
        if(!g_imManager)
        {
            g_imManager = new ImagemapManager();
        }
        return g_imManager;
    }
    void ImagemapManager::DetoryInstance()
    {
        if(g_imManager)
        {
            delete g_imManager;
            g_imManager = nullptr;
        }
    }
    ImagemapManager::ImagemapManager(/* args */)
    {

    }
    ImagemapManager::~ImagemapManager()
    {
        for (auto iter = m_imagemaps.begin() ; iter != m_imagemaps.end(); iter++)
        {
            delete iter->second;
        }
        m_imagemaps.clear();
    }
    Imagemap* ImagemapManager::GetImagemap(const std::string& name)
    {
        Imagemap* result = nullptr;
        auto iter = m_imagemaps.find(name);
        if(iter != m_imagemaps.end())
            result = iter->second;
        return result;
    }
    Imagemap* ImagemapManager::LoadImagemap(const std::string& path, const std::string& fileName)
    {
        int x,y,n;
        unsigned char *data = stbi_load((path+fileName).c_str(), &x, &y, &n, 0); // TODO EXTENSION for hdr
        lightman::Imagemap* result = nullptr;
        if (data)
        {
            assert((n==1||n==3||n==4));

            lightman::Imagemap::ComponentType type = n == 1 ? lightman::Imagemap::ComponentType::R8 :
                (n == 3 ? lightman::Imagemap::ComponentType::RGB8 : lightman::Imagemap::ComponentType::RGBA8);
            result = new lightman::Imagemap((void*)data, x, y, type);
            stbi_image_free(data);

            m_imagemaps.insert({fileName, result});
        }
        return result;
    }
} // namespace lightman
