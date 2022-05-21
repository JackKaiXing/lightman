#ifndef _LIGHTMAN_IMAGEMAPTEXTURE_H
#define _LIGHTMAN_IMAGEMAPTEXTURE_H

#include "texture/texture.h"
#include "texture/imagemap.h"

namespace lightman
{
    class ImagemapTexture : public Texture
    {
    public:
        ImagemapTexture(){};
        ImagemapTexture(const std::string& name) : Texture(name){};
        ~ImagemapTexture(){};

        TextureType GetType() const override {return TextureType::IMAGEMAP_TEX;};
        void SetImageMap(Imagemap* im);
    private:
        Imagemap * m_imagemap = nullptr;
    };
}

#endif _LIGHTMAN_IMAGEMAPTEXTURE_H
