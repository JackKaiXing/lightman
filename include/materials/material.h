#ifndef _LIGHTMAN_MATERIALS_H
#define _LIGHTMAN_MATERIALS_H

#include "backend/driverbase.h"
#include "materials/materialinstance.h"
#include "utils/ref.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class MaterialInstance;
    class Material : public utils::Ref
    {
    public:
        // for convenient usage of enum, https://riptutorial.com/c/example/6564/typedef-enum
        typedef enum
        {
            MATTE, GLASS
        }MaterialType;
    public:
        Material() = default;
        Material(const std::string& name);
        virtual ~Material() = default;
        virtual MaterialType getMaterialType() = 0;
    friend class MaterialManager;
    protected:
        MaterialInstance* createMaterialInstance(const std::string& name);
    private:
        backend::HwProgram * m_program = nullptr;
        std::string m_name;
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATERIALS_H