
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
            result = iter->second->img;
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

             // when loaded, no need to create hardware object, so set to nullptr
            ImagemapData *imgage = new ImagemapData(result, nullptr);
            m_imagemaps.insert({fileName, imgage});
        }
        return result;
    }
    backend::HwTexture* ImagemapManager::GetHwTexture(const std::string& name)
    {
        // TODO STORE EVERY CREATED TEXTURE?
        backend::HwTexture* result = nullptr;

        auto iter = m_imagemaps.find(name);
        if(iter != m_imagemaps.end())
        {
            Imagemap* img = iter->second->img;
            if ( ! iter->second->texture)
            {
                iter->second->texture = Engine::GetInstance()->GetDriver()->createTexture(backend::SamplerType::SAMPLER_2D, 
                    1, 
                    img->GetTextureFormat(), 1, 
                    img->GetWidth(), 
                    img->GetHeight(), 
                    1, 
                    backend::TextureUsage::SAMPLEABLE);
                Engine::GetInstance()->GetDriver()->update2DImage(iter->second->texture, 0,
                    0, 0,
                    img->GetWidth(), img->GetHeight(),
                    img->GetData(), img->GetDataSize(), 
                    img->GetPixelFormat(), img->GetPixelType());
            }
            result = iter->second->texture;
        }

        return result;
    }
} // namespace lightman
