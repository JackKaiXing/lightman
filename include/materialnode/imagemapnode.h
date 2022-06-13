#ifndef _LIGHTMAN_IMAGEMAPTEXTURE_H
#define _LIGHTMAN_IMAGEMAPTEXTURE_H

#include "texture/texture.h"
#include "texture/imagemap.h"
#include "backend/driverbase.h"

namespace lightman
{
    class ImagemapTexture : public Texture
    {
    public:
        ImagemapTexture(){};
        ImagemapTexture(const std::string& name) : Texture(name){};
        ~ImagemapTexture(){};

        TextureType GetType() const override {return TextureType::IMAGEMAP_TEX;};
        void GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const override;
        backend::UniformType GetShaderString(std::string& result) const override;
        void SetImageMap(Imagemap* im);
        bool IsAllowedExposeAsUniform() const override {return true;};

        void setExposeAsUniform(bool enable);
    private:
        Imagemap * m_imagemap = nullptr;
        bool m_enableExpose = false;
    };
}

#endif // _LIGHTMAN_IMAGEMAPTEXTURE_H
