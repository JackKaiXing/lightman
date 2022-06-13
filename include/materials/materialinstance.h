#ifndef _LIGHTMAN_MATERIALINSTANCE_H
#define _LIGHTMAN_MATERIALINSTANCE_H

#include "materials/material.h"
#include "math/matrix4x4.h"
#include "utils/ref.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class Material;
    class Engine;
    class MaterialInstance : public utils::Ref
    {
    public:
        struct Blob
        {
            uint32_t dataSize;
            void* data = nullptr;
        };

    public:
        Material* GetMaterial();
        void Commit();
        template<typename T>
        void SetParameter(const char* name, T const& value, uint32_t count = 0);
        void SetParameterNoType(const char* name, void* value, uint32_t count);
        void SetParameterImpl(uint32_t offset, uint32_t size, void* data);

    friend class Material;
    protected:
        MaterialInstance(Material * material, const std::string& name);
        ~MaterialInstance();
    private:
        std::string m_name;
        Material * m_material = nullptr;
        backend::HwBufferObject* m_uniformBufferHw = nullptr;
        Blob m_uniforBufferCPU;
        bool m_needToUpdateUniformBuffer = true;
        backend::HwSamplerGroup* m_SamplerGroup = nullptr;
    };
    
    template<> void MaterialInstance::SetParameter<float> (const char* name, float const& value, uint32_t count);
    template<> void MaterialInstance::SetParameter<math::Matrix4X4> (const char* name, math::Matrix4X4 const& value, uint32_t count);
    template<> void MaterialInstance::SetParameter<int> (const char* name, int const& value, uint32_t count);
    template<> void MaterialInstance::SetParameter<math::Vector3> (const char* name, math::Vector3 const& value, uint32_t count);
    template<> void MaterialInstance::SetParameter<math::Vector4> (const char* name, math::Vector4 const& value, uint32_t count);
    
} // namespace lightman
#endif // _LIGHTMAN_MATERIALINSTANCE_H
