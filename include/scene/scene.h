#ifndef _LIGHTMAN_INSTANCEDOBJECT_H
#define _LIGHTMAN_INSTANCEDOBJECT_H

#include <string>

using namespace std;

namespace lightman
{
    // ----------------------------------------------------------------------------
    namespace scene
    {
        class Scene
        {
        public:
            void Parse(const string& file);
        private:
                
        };
    } // namespace scene
} // namespace lightman

#endif // #ifndef _LIGHTMAN_INSTANCEDOBJECT_H