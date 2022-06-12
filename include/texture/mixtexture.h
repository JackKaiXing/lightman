#ifndef _LIGHTMAN_MIXTEXTURE_H
#define _LIGHTMAN_MIXTEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class MixTexture : public Texture
    {
    public:
        MixTexture() = delete;
        MixTexture(const std::string& name) : Texture(name){};
        ~MixTexture(){};
        void SetAmount(const Texture * amount);
        void SetTex1(const Texture * tex1);
        void SetTex2(const Texture * tex2);

        TextureType GetType() const override {return TextureType::MIX_TEX;};
        void GetBlockInfo(std::vector<UniformDefine>& uDefines) const override;
        backend::UniformType GetShaderString(std::string& result) const override;
        bool IsAllowedExposeAsUniform() const override {return false;};
    private:
        const Texture* m_amount;
	    const Texture* m_tex1;
	    const Texture* m_tex2;
    };
}

#endif // _LIGHTMAN_MIXTEXTURE_H
