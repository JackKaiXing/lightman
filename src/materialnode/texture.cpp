
#include "materialnode/node.h"

namespace lightman
{
    bool Texture::CheckTypeForBinaryOperation(backend::UniformType type1, backend::UniformType type2, backend::UniformType& result)
    {
        bool isTypeCompatible = false;

        switch (type1) {
            case backend::UniformType::BOOL:
                {
                    if ( type2 == backend::UniformType::BOOL  ||
                         type2 == backend::UniformType::BOOL2 || 
                         type2 == backend::UniformType::BOOL3 ||
                         type2 == backend::UniformType::BOOL4 )
                    {
                        result = type2;
                        isTypeCompatible =  true;
                    }
                }
                break;
            case backend::UniformType::BOOL2:
                {
                    if ( type2 == backend::UniformType::BOOL  ||
                         type2 == backend::UniformType::BOOL2 )
                    {
                        result = backend::UniformType::BOOL2;
                        isTypeCompatible =  true;
                    }
                }
                break;
            case backend::UniformType::BOOL3:
                {
                    if ( type2 == backend::UniformType::BOOL  ||
                         type2 == backend::UniformType::BOOL3 )
                    {
                        result = backend::UniformType::BOOL3;
                        isTypeCompatible =  true;
                    }
                }
                break;
            case backend::UniformType::BOOL4:
                {
                    if ( type2 == backend::UniformType::BOOL  ||
                         type2 == backend::UniformType::BOOL4 )
                    {
                        result = backend::UniformType::BOOL4;
                        isTypeCompatible =  true;
                    }
                }
                break;
            case backend::UniformType::FLOAT:
                {
                    if ( type2 == backend::UniformType::FLOAT  ||
                         type2 == backend::UniformType::FLOAT2 || 
                         type2 == backend::UniformType::FLOAT3 ||
                         type2 == backend::UniformType::FLOAT4 )
                    {
                        result = type2;
                        isTypeCompatible =  true;
                    }
                }
                break;
            case backend::UniformType::FLOAT2:
                {
                    if ( type2 == backend::UniformType::FLOAT  ||
                         type2 == backend::UniformType::FLOAT2 )
                    {
                        result = backend::UniformType::FLOAT2;
                        isTypeCompatible =  true;
                    }
                }
                break;
            case backend::UniformType::FLOAT3:
                {
                    if ( type2 == backend::UniformType::FLOAT  ||
                         type2 == backend::UniformType::FLOAT3 )
                    {
                        result = backend::UniformType::FLOAT3;
                        isTypeCompatible =  true;
                    }
                }
                break;
            case backend::UniformType::FLOAT4:
                {
                    if ( type2 == backend::UniformType::FLOAT  ||
                         type2 == backend::UniformType::FLOAT4 )
                    {
                        result = backend::UniformType::FLOAT4;
                        isTypeCompatible =  true;
                    }
                }
                break;
            case backend::UniformType::INT:
            case backend::UniformType::INT2:
            case backend::UniformType::INT3:
            case backend::UniformType::INT4:

            case backend::UniformType::UINT:
            case backend::UniformType::UINT2:
            case backend::UniformType::UINT3:
            case backend::UniformType::UINT4:

            case backend::UniformType::MAT3:
            case backend::UniformType::MAT4:
                return 4;
        }

        return isTypeCompatible;
    }
} // namespace lightman
