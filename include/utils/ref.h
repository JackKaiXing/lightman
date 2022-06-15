#ifndef _LIGHTMAN_REF_H
#define _LIGHTMAN_REF_H

#include <cstdint>

namespace lightman
{
    namespace utils
    {
        #define RELEASEORDELETE(target) \
        { \
            if(target->GetRefCount() == 1) \
                delete target; \
            else \
                target->ReleaseRef(); \
        }

        #define RELEASEANDRETURN(target) \
        { \
            if (target->GetRefCount() > 1) \
            { \
                target->ReleaseRef(); \
                return; \
            } \
        } \

        class Ref
        {
        public:
            Ref();
            virtual ~Ref();
            void ReleaseRef();
            uint32_t GetRefCount();
            void IncreaseRef();
        private:
            uint32_t m_refCount;
        };
    }
}
#endif // _LIGHTMAN_REF_H