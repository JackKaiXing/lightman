
#include "utils/fileoperator.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <sstream>

namespace lightman
{
    namespace utils
    {
        std::string FileOperator::GetFileAsString(const std::string filepath)
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
    }
}
