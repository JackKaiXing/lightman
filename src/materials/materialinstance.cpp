
#include "materials/materialinstance.h"

namespace lightman
{
    MaterialInstance::MaterialInstance(Material * material, const std::string& name)
    {
        m_name = name;
        m_material = material;
    }
    MaterialInstance::~MaterialInstance()
    {

    }
} // namespace lightman
