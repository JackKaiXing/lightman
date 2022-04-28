#ifndef _LIGHTMAN_MATERIALINSTANCE_H
#define _LIGHTMAN_MATERIALINSTANCE_H

#include "materials/material.h"
#include "math/matrix4x4.h"
#include "utils/ref.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class Material;
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
        void SetParameter(const char* name, T const& value);
        void SetParameterImpl(uint32_t offset, uint32_t size, void* data);
        void BindUniformBlockToProgram(backend::HwProgram* program);

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
    };
    
    template<> void MaterialInstance::SetParameter<float> (const char* name, float const& value);
    template<> void MaterialInstance::SetParameter<math::Matrix4X4> (const char* name, math::Matrix4X4 const& value);

    
} // namespace lightman
#endif // _LIGHTMAN_MATERIALINSTANCE_H