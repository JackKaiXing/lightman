
#ifndef _LIGHTMAN_SHADERMATERIAL_H
#define _LIGHTMAN_SHADERMATERIAL_H

#include <string>
#include "materials/material.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    namespace ShaderString
    {
        std::string GetVertexAttribute();
        std::string CreateBlockInfo(const std::vector<UniformDefine>& uDefine);
    } // namespace ShaderString    
} // namespace lightman
#endif // _LIGHTMAN_SHADERMATERIAL_H
