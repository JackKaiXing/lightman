#include "utils/ref.h"

#include <assert.h>

namespace lightman
{
    namespace utils
    {
        Ref::Ref()
        {
            m_refCount = 1;
        }
        Ref::~Ref()
        {
            m_refCount--;
            assert(m_refCount==0);
        }
        void Ref::Increase()
        {
            m_refCount++;
        }
        void Ref::Release()
        {
            assert(!(m_refCount==1));
            m_refCount--;
        }
        uint32_t Ref::GetRefCount()
        {
            return m_refCount;
        }
    }
}