#ifndef _LIGHTMAN_IMAGEMAPTEXTURE_H
#define _LIGHTMAN_IMAGEMAPTEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class ImagemapTexture : public Texture
    {
    public:
        ImagemapTexture(){};
        ImagemapTexture(const std::string& name) : Texture(name){};
        ~ImagemapTexture(){};

        TextureType GetType() const override {return TextureType::IMAGEMAP_TEX;};
    private:
    };
}

#endif _LIGHTMAN_IMAGEMAPTEXTURE_H
