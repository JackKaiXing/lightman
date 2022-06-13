#ifndef _LIGHTMAN_TEXTURE_H
#define _LIGHTMAN_TEXTURE_H

#include <string>
#include <vector>
#include "assert.h"

#include "materials/uniformdefines.h"
#include "materials/shader.h"
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
        virtual void GetBlockInfo(std::vector<UniformDefine>& uDefines, std::vector<SamplerDefine>& sDefines) const = 0;
        virtual backend::UniformType GetShaderString(std::string& result) const = 0;
        virtual bool IsAllowedExposeAsUniform() const = 0;
        std::string GetName() const {return m_name;};

        static bool CheckTypeForBinaryOperation(backend::UniformType type1, backend::UniformType type2, backend::UniformType& result);
        
    private:
        std::string m_name;
    };
}
#endif //_LIGHTMAN_TEXTURE_H
