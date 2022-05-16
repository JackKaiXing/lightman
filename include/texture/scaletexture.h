#ifndef _LIGHTMAN_SCALETEXTURE_H
#define _LIGHTMAN_SCALETEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class ScaleTexture : public Texture
    {
    public:
        ScaleTexture(){};
        ScaleTexture(const std::string& name) : Texture(name){};
        ~ScaleTexture(){};

        TextureType GetType() const override {return TextureType::SCALE_TEX;};
    private:
    };
}

#endif _LIGHTMAN_SCALETEXTURE_H
