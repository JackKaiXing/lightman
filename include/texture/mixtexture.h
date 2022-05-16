#ifndef _LIGHTMAN_MIXTEXTURE_H
#define _LIGHTMAN_MIXTEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class MixTexture : public Texture
    {
    public:
        MixTexture(){};
        MixTexture(const std::string& name) : Texture(name){};
        ~MixTexture(){};

        TextureType GetType() const override {return TextureType::MIX_TEX;};
    private:
    };
}

#endif _LIGHTMAN_MIXTEXTURE_H
