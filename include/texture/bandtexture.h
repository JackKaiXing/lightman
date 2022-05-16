#ifndef _LIGHTMAN_BANDTEXTURE_H
#define _LIGHTMAN_BANDTEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class BandTexture : public Texture
    {
    public:
        BandTexture(){};
        BandTexture(const std::string& name) : Texture(name){};
        ~BandTexture(){};

        TextureType GetType() const override {return TextureType::BAND_TEX;};
    private:
    };
}

#endif _LIGHTMAN_BANDTEXTURE_H
