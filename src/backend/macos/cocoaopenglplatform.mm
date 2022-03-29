
#include <stdio.h>

#include "backend/macos/cocoaopenglplatform.h"

namespace lightman
{
    namespace backend
    {
        CocoaOpenglPlatform::CocoaOpenglPlatform()
        {
            printf("hello cocoa\n");
        }

        backend::Driver* CocoaOpenglPlatform::CreateDriver() noexcept
        {
            return nullptr;
        }
    }
}