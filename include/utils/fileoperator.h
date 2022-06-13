#ifndef _LIGHTMAN_FILEOPERATOR_H
#define _LIGHTMAN_FILEOPERATOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <sstream>

namespace lightman
{
    namespace utils
    {
        namespace FileOperator
        {
            inline std::string GetFileAsString(const std::string filepath)
            {
                // https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
                std::ifstream infile;
                infile.open(filepath);
                if(!infile.is_open())
                    assert(0);
                
                std::string result((std::istreambuf_iterator<char>(infile)),
                std::istreambuf_iterator<char>());
                return result;
            }
            inline std::string GetFileExtensionAsString(const std::string filename)
            {
                std::string result;
                int pos = filename.find_last_of(".");
                result = filename.substr(pos+1, filename.length() -1 );

                return result;
            }
        };
    }
}
#endif // _LIGHTMAN_FILEOPERATOR_H