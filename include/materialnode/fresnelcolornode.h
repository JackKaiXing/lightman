#ifndef _LIGHTMAN_FRESNELCOLORTEXTURE_H
#define _LIGHTMAN_FRESNELCOLORTEXTURE_H

#include "materialnode/node.h"

namespace lightman
{
    class FresnelColorTexture : public Texture
    {
    public:
        FresnelColorTexture() = delete;
        FresnelColorTexture(const std::string& name) : Texture(name){};
        ~FresnelColorTexture(){};

        void SetKr(const Texture* kr);

        TextureType GetType() const override {return TextureType::FRESNELCOLOR_TEX;};
        void GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const override;
        backend::UniformType GetShaderString(std::string& result) const override;
        bool IsAllowedExposeAsUniform() const override {return false;};

    private:
        const Texture* m_Kr;
    };
}

#endif // _LIGHTMAN_FRESNELCOLORTEXTURE_H
