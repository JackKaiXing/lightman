#ifndef _LIGHTMAN_MATERIALS_H
#define _LIGHTMAN_MATERIALS_H

#include <vector>
#include <unordered_map>

#include "backend/driverbase.h"
#include "materials/materialinstance.h"
#include "utils/ref.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    struct UniformDefine
    {
        std::string name;
        backend::UniformType type;
        uint32_t size;
        backend::Precision precision;
    };

    class MaterialInstance;
    class Material : public utils::Ref
    {
    public:
        // for convenient usage of enum, https://riptutorial.com/c/example/6564/typedef-enum
        typedef enum
        {
            MATTE, GLASS,
            MAX_MATERIALTYPE_COUNT
        }MaterialType;

        // https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
        struct UniformInfo {
            std::string name;// name of this uniform
            uint16_t offset;    // offset in "uint32_t" of this uniform in the buffer
            uint8_t stride;     // stride in "uint32_t" to the next element
            backend::UniformType type;          // type of this uniform
            uint32_t size;      // size of the array in elements, or 1 if not an array
            backend::Precision precision;// precision of this uniform
            // returns offset in bytes of this uniform (at index if an array)
            inline size_t getBufferOffset(size_t index = 0) const {
                assert(index < size);
                return (offset + stride * index) * sizeof(uint32_t);
            }
        };
        bool IsUniformEmpty() const {return m_uniformsInfoList.empty();};
        uint32_t GetUniformBlockSize() {return m_uniformsSize;};
        void InitUniformBlockInfo(const std::vector<UniformDefine> uDefines);
        static uint8_t BaseAlignmentForType(backend::UniformType type) noexcept;
        static uint8_t StrideForType(backend::UniformType type) noexcept;
        bool HasUniform(const std::string name);
        void GetUniformOffsetAndStrideByName(const std::string name, uint32_t& offset, uint32_t& stride);

    public:
        Material() = default;
        Material(const std::string& name);
        virtual ~Material() = default;
        virtual MaterialType getMaterialType() = 0;
        static uint32_t GetProgramIndexBySupportedVertexAttribute(bool hasTangent, bool hasUV0, bool hasUV1);
        static MaterialType StringToMaterialType(const std::string& name);
    friend class MaterialManager;
    protected:
        MaterialInstance* createMaterialInstance(const std::string& name);
        std::string m_name;
        std::vector<UniformInfo> m_uniformsInfoList;
        uint32_t m_uniformsSize; // size in bytes
        std::unordered_map<std::string, uint32_t> m_uniformInfoMap; 
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATERIALS_H
