#ifndef _LIGHTMAN_REF_H
#define _LIGHTMAN_REF_H

#include <cstdint>

namespace lightman
{
    namespace utils
    {
        class Ref
        {
        public:
            Ref();
            ~Ref();
            void Release();
            uint32_t GetRefCount();
            void Increase();
        private:
            uint32_t m_refCount;
        };
    }
}
#endif // _LIGHTMAN_REF_H