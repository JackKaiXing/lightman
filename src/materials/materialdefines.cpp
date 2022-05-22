
#include "materials/materialdefines.h"
#include "magicenum/magic_enum.hpp"
#include "nlohmann/json.hpp"
#include "utils/fileoperator.h"

using json = nlohmann::json;

namespace lightman
{
    MaterialDefines* g_md = nullptr;

    void MaterialDefines::InitInstance(const std::string& path)
    {
        if(!g_md)
        {
            g_md = new MaterialDefines(path);
        }
    }
    MaterialDefines* MaterialDefines::GetInstance()
    {
        if(g_md)
            return g_md;
        return nullptr;
    }
    void MaterialDefines::DestoryInstance()
    {
        if (g_md)
        {
            delete g_md;
            g_md = nullptr;
        }   
    }
    MaterialDefines::MaterialDefines(const std::string& path)
    {
        std::string fileName;
        std::string filePath;
        std::string jsonString;

        // HARD CODE: load all the defines for different types of material
        for (Material::MaterialType type = (Material::MaterialType)0; 
            type < Material::MaterialType::MAX_MATERIALTYPE_COUNT; 
            type = (Material::MaterialType)(uint32_t(type) + 1))
        {
            fileName = magic_enum::enum_name(type).data();
            std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
            filePath = path + fileName + ".json";

            jsonString = utils::FileOperator::GetFileAsString(filePath);
            auto j3 = json::parse(jsonString);
            std::cout << j3["xingkai"] << std::endl;
        }
    }
    MaterialDefines::~MaterialDefines()
    {
        
    }
} // namespace lightman
