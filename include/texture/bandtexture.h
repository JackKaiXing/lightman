#ifndef _LIGHTMAN_BANDTEXTURE_H
#define _LIGHTMAN_BANDTEXTURE_H

#include "texture/texture.h"

namespace lightman
{
    class BandTexture : public Texture
    {
    public:
        BandTexture() = delete;
        BandTexture(const std::string& name) : Texture(name){};
        ~BandTexture(){};
        void SetAmount(const Texture* amount);

        TextureType GetType() const override {return TextureType::BAND_TEX;};
    private:
        const Texture* m_amount;
    };
}

#endif _LIGHTMAN_BANDTEXTURE_H
