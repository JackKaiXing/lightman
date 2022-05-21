#ifndef _LIGHTMAN_CONSTFLOAT3TEXTURE_H
#define _LIGHTMAN_CONSTFLOAT3TEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class ConstFloat3Texture : public Texture
    {
    public:
        ConstFloat3Texture(){};
        ConstFloat3Texture(const std::string& name) : Texture(name){};
        ~ConstFloat3Texture(){};

        TextureType GetType() const override {return TextureType::CONSTFLOAT3_TEX;};
    private:
    };
}

#endif //_LIGHTMAN_CONSTFLOAT3TEXTURE_H
