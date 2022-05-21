#ifndef _LIGHTMAN_TEXTURE_H
#define _LIGHTMAN_TEXTURE_H

#include <string>
#include "assert.h"

#include "utils/ref.h"

namespace lightman
{
    typedef enum{
        CONSTFLOAT_TEX, CONSTFLOAT3_TEX, IMAGEMAP_TEX, MIX_TEX, SCALE_TEX, SUBTRACT_TEX,

        // procedural
        BAND_TEX, 

        // fresnel
        FRESNELCOLOR_TEX,
    }TextureType;

    class Texture : public utils::Ref
    {
    public:
        Texture(){};
        Texture(const std::string& name) : m_name(name){};
        virtual ~Texture(){};

        virtual TextureType GetType() const = 0;
        
    private:
        std::string m_name;
    };
}
#endif //_LIGHTMAN_TEXTURE_H
