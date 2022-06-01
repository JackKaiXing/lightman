
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
        std::string CreateBlockInfo(const std::vector<UniformDefine>& uDefine, const std::string& UnoformBlockName);
        backend::UniformBlockInfo GetUniformBufferInfo();
        void GetSharedBlockInfo(std::vector<UniformDefine>& uDefines);
        const std::string GetDisneyVertexShader();
        const std::string GetDisneyFragmentShader(const std::string& updateMaterialString);
    } // namespace ShaderString    
} // namespace lightman
#endif // _LIGHTMAN_SHADERMATERIAL_H
