#ifndef _LIGHTMAN_HANDLE_H
#define _LIGHTMAN_HANDLE_H

#include <limits>
#include <algorithm>
#include <assert.h>

namespace lightman
{
    namespace backend
    {
        class HandleBase
        {
            public:
            using HandleID = uint32_t;
            // https://en.cppreference.com/w/cpp/language/copy_initialization
            // https://en.cppreference.com/w/cpp/language/direct_initialization
            // https://en.cppreference.com/w/cpp/language/list_initialization
            // copy-list-initialization
            static constexpr const HandleID nullid = HandleID{ std::numeric_limits<HandleID>::max() };

            // https://en.cppreference.com/w/cpp/language/constexpr
            constexpr HandleBase() noexcept: object(nullid) {}

            explicit HandleBase(HandleID id) noexcept : object(id) {
                assert(object != nullid);
            }

            // https://en.cppreference.com/w/cpp/language/default_constructor
            // https://en.cppreference.com/w/cpp/language/copy_constructor
            // default constuctor in the form of copy constructor
            HandleBase(HandleBase const& rhs) noexcept = default;

            //https://en.cppreference.com/w/cpp/language/move_constructor
            HandleBase(HandleBase&& rhs) noexcept : object(rhs.object) {
                rhs.object = nullid;
            }

            // https://en.cppreference.com/w/cpp/language/copy_assignment
            HandleBase& operator=(HandleBase const& rhs) noexcept = default;
            // https://en.cppreference.com/w/cpp/language/move_assignment
            HandleBase& operator=(HandleBase&& rhs) noexcept {
                std::swap(object, rhs.object);
                return *this;
            }

            // https://en.cppreference.com/w/cpp/language/implicit_conversion, to enable if(HandleBase), otherwise compile error
            explicit operator bool() const noexcept { return object != nullid; }

            void clear() noexcept { object = nullid; }

            bool operator==(const HandleBase& rhs) const noexcept { return object == rhs.object; }
            bool operator!=(const HandleBase& rhs) const noexcept { return object != rhs.object; }

            HandleID getId() const noexcept { return object; }
        private:
            HandleID object;
        };

        template <typename B>
        struct Handle : public HandleBase
        {
            // https://en.cppreference.com/w/cpp/types/enable_if
            // https://en.cppreference.com/w/cpp/types/is_base_of
            // it could be initialized by its derived class.
            template<typename D, typename = std::enable_if_t< std::is_base_of(B,D) >
            Handle(Handle<D> const& derived) : HandleBase(derived){}
        };
    }
}
#endif // _LIGHTMAN_HANDLE_H