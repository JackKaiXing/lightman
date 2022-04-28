
#include "materials/materialinstance.h"
#include "engine/engine.h"

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
    }
    MaterialInstance::~MaterialInstance()
    {
        if (m_uniforBufferCPU.data)
        {
            std::free(m_uniforBufferCPU.data);
        }
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
    }
    void MaterialInstance::SetParameterImpl(uint32_t offset, uint32_t size, void* data)
    {
        assert(offset + size <= m_uniforBufferCPU.dataSize);
        std::memcpy((uint8_t*)m_uniforBufferCPU.data + offset, data, size);
    }
    template<>
    void MaterialInstance::SetParameter(const char* name, float const& value)
    {
        if (m_material->HasUniform(name))
        {
            // TO Remove Repeat
            uint32_t stride = 0, offset = 0;
            m_material->GetUniformOffsetAndStrideByName(name, offset, stride);
            SetParameterImpl(offset, stride, (void*)&value);
            m_needToUpdateUniformBuffer = true;
        }
        m_needToUpdateUniformBuffer = true;
    }
    template<>
    void MaterialInstance:: SetParameter(const char* name, math::Matrix4X4 const& value)
    {
        if (m_material->HasUniform(name))
        {
            uint32_t stride = 0, offset = 0;
            m_material->GetUniformOffsetAndStrideByName(name, offset, stride);
            SetParameterImpl(offset, stride, (void*)value.m_value);
            m_needToUpdateUniformBuffer = true;
        }
    }
    void MaterialInstance::BindUniformBlockToProgram(backend::HwProgram* program)
    {
        // TODO use the program? to remove const binding location
        Engine::GetInstance()->GetDriver()->bindUniformBuffer(0, m_uniformBufferHw);
    }

} // namespace lightman
