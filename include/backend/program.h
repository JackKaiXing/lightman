#ifndef _LIGHTMAN_PROGRAM_H
#define _LIGHTMAN_PROGRAM_H

#include <cstddef>
#include <cstdint>
#include <array>

namespace lightman
{
    namespace backend
    {
        class Program
        {
        public:
            static constexpr size_t SHADER_TYPE_COUNT = 2;
            enum class Shader : uint8_t
            {
                VERTEX = 0,
                FRAGMENT = 1
            };
            struct ShaderBlob
            {
                uint32_t size;
                uint8_t * dat;
            };
            std::array<ShaderBlob,SHADER_TYPE_COUNT> const& getShadersSource() const noexcept { return m_shaderSource;}
        private:
            std::array<ShaderBlob,SHADER_TYPE_COUNT> m_shaderSource;
        };
    }
}
#endif // _LIGHTMAN_PROGRAM_H