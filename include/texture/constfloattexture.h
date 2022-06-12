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
        void GetBlockInfo(std::vector<UniformDefine>& uDefines) const override;
        backend::UniformType GetShaderString(std::string& result) const override;
        bool IsAllowedExposeAsUniform() const override {return true;};

        void setExposeAsUniform(bool enable);
    private:
        float m_value;
        bool m_enableExpose = false;
    };
}

#endif //_LIGHTMAN_CONSTFLOATTEXTURE_H
