#ifndef _LIGHTMAN_FRESNELCOLORTEXTURE_H
#define _LIGHTMAN_FRESNELCOLORTEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class FresnelColorTexture : public Texture
    {
    public:
        FresnelColorTexture() = delete;
        FresnelColorTexture(const std::string& name) : Texture(name){};
        ~FresnelColorTexture(){};

        void SetKr(const Texture* kr);

        TextureType GetType() const override {return TextureType::FRESNELCOLOR_TEX;};
    private:
        const Texture* m_Kr;
    };
}

#endif _LIGHTMAN_FRESNELCOLORTEXTURE_H