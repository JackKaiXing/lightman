
#include "managers/imagemapmanager.h"
#include "stb/stb_image.h"
#include "assert.h"
#include "engine/engine.h"
#include "utils/fileoperator.h"

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
        unsigned char *data = stbi_load((path+fileName).c_str(), &x, &y, &n, 0);
        lightman::Imagemap* result = nullptr;
        if (data)
        {
            assert((n==1||n==3||n==4));

            std::string ext = utils::FileOperator::GetFileExtensionAsString(fileName);
            if (ext == "png")
            {
                backend::TextureFormat type = n == 1 ? backend::TextureFormat::R8 :
                (n == 3 ? backend::TextureFormat::RGB8 : backend::TextureFormat::RGBA8);
                backend::PixelDataFormat pformat = n == 1 ? backend::PixelDataFormat::R :
                (n == 3 ? backend::PixelDataFormat::RGB : backend::PixelDataFormat::RGBA);
                result = new lightman::Imagemap(fileName, (void*)data, x, y, type, pformat, backend::PixelDataType::UBYTE,
                                                backend::SamplerType::SAMPLER_2D);
            }
            else if (ext == "hdr")
            {

            }
            
            stbi_image_free(data);

            m_imagemaps.insert({fileName, result});
        }
        return result;
    }
} // namespace lightman
