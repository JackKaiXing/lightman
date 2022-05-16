#ifndef _LIGHTMAN_TEXTURE_H
#define _LIGHTMAN_TEXTURE_H

#include <string>
#include "assert.h"

#include "utils/ref.h"

namespace lightman
{
    typedef enum{
        IMAGEMAP_TEX, MIX_TEX, SCALE_TEX, SUBTRACT_TEX,

        // procedural
        BAND_TEX, 

        // fresnel
        FRESNELCOLOR_TEX,
    }TextureType;

    class Texture : public utils::Ref
    {
    public:
    static TextureType StringToTextureType(const std::string& name);
    public:
        Texture(){};
        Texture(const std::string& name) : m_name(name){};
        virtual ~Texture(){};

        virtual TextureType GetType() const = 0;
        
    private:
        std::string m_name;
    };

    TextureType Texture::StringToTextureType(const std::string& name)
    {
        if(name == "imagemap")
            return TextureType::IMAGEMAP_TEX;
        
        if(name == "mix")
            return TextureType::MIX_TEX;
        
        if(name == "scale")
            return TextureType::SCALE_TEX;
        
        if(name == "subtract")
            return TextureType::SUBTRACT_TEX;
        
        if(name == "band")
            return TextureType::BAND_TEX;

        if(name == "fresnelcolor")
            return TextureType::FRESNELCOLOR_TEX;
        
        assert(0);
    }
}
#endif _LIGHTMAN_TEXTURE_H
