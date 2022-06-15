
#include "materials/materialinstance.h"
#include "engine/engine.h"
#include "managers/imagemapmanager.h"

namespace lightman
{
    MaterialInstance::MaterialInstance(Material * material, const std::string& name)
    {
        m_name = name;
        m_material = material;

        if(! m_material->IsUniformEmpty())
        {
            m_uniforBufferCPU.dataSize = m_material->GetUniformBlockSize();
            m_uniforBufferCPU.data = std::malloc(m_uniforBufferCPU.dataSize);

            m_uniformBufferHw = Engine::GetInstance()->GetDriver()->createBufferObject(
                m_uniforBufferCPU.dataSize, backend::BufferObjectBinding::UNIFORM, backend::BufferUsage::DYNAMIC);
        }
        // currently we have only one uniformblock so default == 0
        Engine::GetInstance()->GetDriver()->bindUniformBuffer(0, m_uniformBufferHw);

        if (! m_material->IsSamplerEmpty())
        {
            auto infos = m_material->GetSamplerInfoList();
            uint32_t size = m_material->GetSamplerBlockSize();
            m_SamplerGroup = Engine::GetInstance()->GetDriver()->createSamplerGroup();
            for (int i = 0; i < size; i++)
            {
                m_SamplerGroup->texs.push_back(ImagemapManager::GetInstance()->GetImagemap(infos.at(i).ImgName)->GetHwTexture());
            }
        }
    }
    MaterialInstance::~MaterialInstance()
    {
        RELEASEANDRETURN(this);
        
        if (m_uniforBufferCPU.data)
        {
            std::free(m_uniforBufferCPU.data);
        }

        if (m_material)
            RELEASEORDELETE(m_material);

        if (m_uniformBufferHw)
            Engine::GetInstance()->GetDriver()->DestroyBufferObject(m_uniformBufferHw);

        if (m_SamplerGroup)
            Engine::GetInstance()->GetDriver()->DestroySamplerGroup(m_SamplerGroup);
    }
    Material* MaterialInstance::GetMaterial()
    {
        return m_material;
    }
    void MaterialInstance::Commit()
    {
        if (m_needToUpdateUniformBuffer)
        {
            Engine::GetInstance()->GetDriver()->updateBufferObject(m_uniformBufferHw, 
                m_uniforBufferCPU.data, m_uniforBufferCPU.dataSize, 0);
            m_needToUpdateUniformBuffer = false;
        }
        if (m_SamplerGroup->texs.size() > 0)
        {
            auto infos = m_material->GetSamplerInfoList();
            for (int i = 0; i < infos.size(); i++)
            {
                Engine::GetInstance()->GetDriver()->bindSamplers(infos.at(i).offset, m_SamplerGroup->texs.at(i));
            }
        }
    }
    void MaterialInstance::SetParameterImpl(uint32_t offset, uint32_t size, void* data)
    {
        assert(offset + size <= m_uniforBufferCPU.dataSize);
        std::memcpy((uint8_t*)m_uniforBufferCPU.data + offset, data, size);
    }
    void MaterialInstance::SetParameterNoType(const char* name, void* value, uint32_t count)
    {
        if (m_material->HasUniform(name))
        {
            uint32_t stride = 0, offset = 0;
            m_material->GetUniformOffsetAndStrideByName(name, offset, stride, count);
            SetParameterImpl(offset, stride, value);
            m_needToUpdateUniformBuffer = true;
        }
    }
    template<>
    void MaterialInstance::SetParameter(const char* name, float const& value, uint32_t count)
    {
        SetParameterNoType(name, (void*)&value, count);
    }
    template<>
    void MaterialInstance::SetParameter(const char* name, math::Matrix4X4 const& value, uint32_t count)
    {
        SetParameterNoType(name, (void*)&value.m_value, count);
    }
    template<>
    void MaterialInstance::SetParameter<int> (const char* name, int const& value, uint32_t count)
    {
        SetParameterNoType(name, (void*)&value, count);
    }
    template<>
    void MaterialInstance::SetParameter<math::Vector3> (const char* name, math::Vector3 const& value, uint32_t count)
    {
        SetParameterNoType(name, (void*)&value, count);
    }
    template<>
    void MaterialInstance::SetParameter<math::Vector4> (const char* name, math::Vector4 const& value, uint32_t count)
    {
        SetParameterNoType(name, (void*)&value, count);
    }
} // namespace lightman
