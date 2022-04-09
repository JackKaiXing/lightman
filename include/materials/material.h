#ifndef _LIGHTMAN_MATERIALS_H
#define _LIGHTMAN_MATERIALS_H

namespace lightman
{
    // ----------------------------------------------------------------------------
    // for convenient usage of enum, https://riptutorial.com/c/example/6564/typedef-enum
    typedef enum
    {
        MATTE, GLASS
    }MaterialType;
    // ----------------------------------------------------------------------------
    class Material
    {
    public:
        Material(){};
        virtual ~Material(){};
        virtual MaterialType getMaterialType() = 0;
    private:
    };
    
} // namespace lightman
#endif // _LIGHTMAN_MATERIALS_H