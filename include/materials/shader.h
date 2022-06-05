
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
        void GetSharedBlockInfo(std::vector<UniformDefine>& uDefines);
        const std::string GetFragmentShaderHead();

        // disney brdf shader
        const std::string GetDisneyVertexShader();
        const std::string GetDisneyFragmentShader(const std::string& updateMaterialString);

        // blender work bench brdf shader
        void GetBlenderBlockInfo(std::vector<UniformDefine>& uDefines);
        const std::string GetBlenderVertexShader();
        const std::string GetBlenderFragmentShader(const std::string& updateMaterialString);
    } // namespace ShaderString    
} // namespace lightman
#endif // _LIGHTMAN_SHADERMATERIAL_H
