#ifndef _LIGHTMAN_SCALETEXTURE_H
#define _LIGHTMAN_SCALETEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class ScaleTexture : public Texture
    {
    public:
        ScaleTexture() = delete;
        ScaleTexture(const std::string& name) : Texture(name){};
        ~ScaleTexture(){};

        void SetTex1(const Texture * tex1);
        void SetTex2(const Texture * tex2);

        TextureType GetType() const override {return TextureType::SCALE_TEX;};
    private:
        const Texture* m_tex1;
	    const Texture* m_tex2;
    };
}

#endif _LIGHTMAN_SCALETEXTURE_H
