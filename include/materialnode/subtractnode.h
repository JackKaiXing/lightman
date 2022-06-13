#ifndef _LIGHTMAN_SUBTRACTTEXTURE_H
#define _LIGHTMAN_SUBTRACTTEXTURE_H

#include "materialnode/node.h"

namespace lightman
{
    class SubtractTexture : public Texture
    {
    public:
        SubtractTexture() = delete;
        SubtractTexture(const std::string& name) : Texture(name){};
        ~SubtractTexture(){};

        void SetTex1(const Texture * tex1);
        void SetTex2(const Texture * tex2);

        TextureType GetType() const override {return TextureType::SUBTRACT_TEX;};
        void GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const override;
        backend::UniformType GetShaderString(std::string& result) const override;
        bool IsAllowedExposeAsUniform() const override {return false;};
    private:
        const Texture* m_tex1;
	    const Texture* m_tex2;
    };
}

#endif // _LIGHTMAN_SUBTRACTTEXTURE_H
