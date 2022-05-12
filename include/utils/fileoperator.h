#ifndef _LIGHTMAN_FILEOPERATOR_H
#define _LIGHTMAN_FILEOPERATOR_H

#include <string>

namespace lightman
{
    namespace utils
    {
        class FileOperator
        {
        public:
            static std::string GetFileAsString(const std::string filepath);
        protected:
            FileOperator();
            ~FileOperator();
        };
    }
}
#endif // _LIGHTMAN_FILEOPERATOR_H