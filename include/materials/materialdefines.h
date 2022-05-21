#ifndef _LIGHTMAN_MATERIALDEFINES_H
#define _LIGHTMAN_MATERIALDEFINES_H

#include "materials/material.h"

namespace lightman
{
    // ----------------------------------------------------------------------------
    class MaterialDefines
    {
    public:
        static void InitInstance(const std::string& path);
        static MaterialDefines* GetInstance();
        static void DestoryInstance();
    protected:
        MaterialDefines(const std::string& path);
        ~MaterialDefines();
    private:
        
    };
} // namespace lightman
#endif // _LIGHTMAN_MATERIALDEFINES_H
