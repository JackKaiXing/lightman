#ifndef _LIGHTMAN_CONSTFLOAT3TEXTURE_H
#define _LIGHTMAN_CONSTFLOAT3TEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class ConstFloat3Texture : public Texture
    {
    public:
        ConstFloat3Texture() = delete;
        ConstFloat3Texture(const std::string& name, float x = 0, float y = 0, float z = 0);
        ~ConstFloat3Texture(){};

        TextureType GetType() const override {return TextureType::CONSTFLOAT3_TEX;};
    private:
        float m_value[3];
    };
}

#endif //_LIGHTMAN_CONSTFLOAT3TEXTURE_H
