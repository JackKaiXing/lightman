#ifndef _LIGHTMAN_FRESNELCOLORTEXTURE_H
#define _LIGHTMAN_FRESNELCOLORTEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class FresnelColorTexture : public Texture
    {
    public:
        FresnelColorTexture(){};
        FresnelColorTexture(const std::string& name) : Texture(name){};
        ~FresnelColorTexture(){};

        TextureType GetType() const override {return TextureType::FRESNELCOLOR_TEX;};
    private:
    };
}

#endif _LIGHTMAN_FRESNELCOLORTEXTURE_H
