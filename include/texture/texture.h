#ifndef _LIGHTMAN_TEXTURE_H
#define _LIGHTMAN_TEXTURE_H

#include <string>
#include "assert.h"

#include "utils/ref.h"

namespace lightman
{
    typedef enum{
        IMAGEMAP
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
            return TextureType::IMAGEMAP;
        
        assert(0);
    }
}
#endif _LIGHTMAN_TEXTURE_H
