#ifndef _LIGHTMAN_SUBTRACTTEXTURE_H
#define _LIGHTMAN_SUBTRACTTEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class SubtractTexture : public Texture
    {
    public:
        SubtractTexture(){};
        SubtractTexture(const std::string& name) : Texture(name){};
        ~SubtractTexture(){};

        TextureType GetType() const override {return TextureType::SUBTRACT_TEX;};
    private:
    };
}

#endif _LIGHTMAN_SUBTRACTTEXTURE_H
