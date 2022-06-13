#ifndef _LIGHTMAN_MATERIALS_H
#define _LIGHTMAN_MATERIALS_H

#include <vector>
#include <unordered_map>

#include "materials/uniformdefines.h"
#include "backend/driverbase.h"
#include "materials/materialinstance.h"
#include "utils/ref.h"
#include "materialnode/node.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class MaterialInstance;
    class Node;
    class Material : public utils::Ref
    {
    public:
        // for convenient usage of enum, https://riptutorial.com/c/example/6564/typedef-enum
        // TODO change from luxcore materials to real-world materials
        typedef enum : uint32_t
        {
            // do not change this, since MaterialDefines need to load from 0 to MAX_MATERIALTYPE_COUNT
            MATTE = 0, 
            MIRROR,
            GLASS,
            ARCHGLASS,
            MIX,
            NULLMAT,
            MATTETRANSLUCENT,
            GLOSSY,
            METAL, 
            ROUGHGLASS, 
            VELVET, 
            CLOTH, 
            CARPAINT, 
            ROUGHMATTE,
            ROUGHMATTETRANSLUCENT, 
            GLOSSYTRANSLUCENT, 
            GLOSSYCOATING, 
            DISNEY,
            TWOSIDED,
            MAX_MATERIALTYPE_COUNT
        }MaterialType;

        // -----------------------------Uniform Info--------------------------------
        // https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
        struct UniformInfo {
            std::string name;   // name of this uniform
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
        static uint8_t BaseAlignmentForType(backend::UniformType type) noexcept;
        static uint8_t StrideForType(backend::UniformType type) noexcept;
        bool HasUniform(const std::string name);
        void GetUniformOffsetAndStrideByName(const std::string name, uint32_t& offset, uint32_t& stride, uint32_t count);

        // -----------------------------Sampler Info--------------------------------
        struct SamplerInfo
        {
            std::string name;
            std::string ImgName;
            uint8_t offset;
            backend::SamplerType type;
            backend::SamplerFormat format;
            bool multiSample = false;
            backend::Precision precision;
        };
         bool IsSamplerEmpty() const {return m_uniformsInfoList.empty();};
         uint32_t GetSamplerBlockSize() {return m_samplersInfoList.size();};
         const std::vector<SamplerInfo>& GetSamplerInfoList(){return m_samplersInfoList;};

    private:
        void InitUniformBlockInfo(const std::vector<UniformDefine> uDefines);
        void InitSamplerBlockInfo(const std::vector<SamplerDefine> sDefines);

    public:
        Material() = default;
        Material(const std::string& name, const Node* bump, const Node* emission);
        virtual ~Material();
        virtual MaterialType getMaterialType() = 0;
        static uint32_t GetProgramIndexBySupportedVertexAttribute(bool hasTangent, bool hasUV0, bool hasUV1);
        virtual bool PrepareForRasterGPU() = 0;
        void PrepareForRasterGPUBase(const std::vector<const Node*>& customTextures, const std::string& UpdateUserMaterialParameters);
        backend::HwProgram* GetProgram() {return m_program;};
        void UpdateDefaultMaterialInstance();
        MaterialInstance* GetDefaultMaterialInstance(){return m_defaultMI;};
    friend class MaterialManager;
    protected:
        MaterialInstance* createMaterialInstance(const std::string& name);
        std::string m_name;
        std::vector<UniformInfo> m_uniformsInfoList;
        uint32_t m_uniformsSize; // size in bytes
        std::unordered_map<std::string, uint32_t> m_uniformInfoMap; 
        std::vector<SamplerInfo> m_samplersInfoList;
        std::unordered_map<std::string, uint32_t> m_samplerInfoMap; 
        
        MaterialInstance* m_defaultMI = nullptr;
        const Node* m_bump = nullptr;            // normal
        const Node* m_emission = nullptr;        // emission
        bool m_isRasterGPUNeedUpdate = false;       // flag for whether we should update raster program.
        // since Material Node is included, there is tiny posibility for program to be shared between materials,so we give a program to each material.
        backend::HwProgram* m_program = nullptr;    // raster program 
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATERIALS_H
