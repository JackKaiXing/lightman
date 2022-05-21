#ifndef _LIGHTMAN_CONSTFLOATTEXTURE_H
#define _LIGHTMAN_CONSTFLOATTEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class ConstFloatTexture : public Texture
    {
    public:
        ConstFloatTexture(){};
        ConstFloatTexture(const std::string& name) : Texture(name){};
        ~ConstFloatTexture(){};

        TextureType GetType() const override {return TextureType::CONSTFLOAT_TEX;};
    private:
    };
}

#endif //_LIGHTMAN_CONSTFLOATTEXTURE_H
