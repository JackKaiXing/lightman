#ifndef _LIGHTMAN_CONSTFLOATTEXTURE_H
#define _LIGHTMAN_CONSTFLOATTEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class ConstFloatTexture : public Texture
    {
    public:
        ConstFloatTexture() = delete;
        ConstFloatTexture(const std::string& name, float v = 0);
        ~ConstFloatTexture(){};

        TextureType GetType() const override {return TextureType::CONSTFLOAT_TEX;};
    private:
        float m_value;
    };
}

#endif //_LIGHTMAN_CONSTFLOATTEXTURE_H
