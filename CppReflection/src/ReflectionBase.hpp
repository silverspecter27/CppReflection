#pragma once
#ifndef REFLECTION_BASE_HPP
#define REFLECTION_BASE_HPP

#include <utility>

#include "constexpr_string.hpp"

#define __REFLECTION_BEGIN__ namespace reflection {
#define __REFLECTION_END__   }
#define __REFLECTION__       ::reflection::

#define CONCATENATE_IMPL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_IMPL(x, y)

__REFLECTION_BEGIN__
namespace allow_access {
    template <class PtrType, PtrType PtrValue, class TagType>
    struct private_access {
        _NODISCARD constexpr friend PtrType Get(TagType) {
            return PtrValue;
        }
    };
}

template <class>
constexpr bool Always_false = false;

template <class Object>
struct Class;

template <class Object>
_NODISCARD constexpr const Class<Object> get_class() noexcept;

namespace test = reflection;
__REFLECTION_END__

namespace cstd {
    template <class Func, class Tuple, std::size_t Index>
    constexpr void For_each_impl(const Tuple& tuple, Func&& func) {
        (void)func(std::get<Index>(tuple));

        if constexpr (Index + 1 < std::tuple_size_v<Tuple>) {
            For_each_impl<Func, Tuple, Index + 1>(tuple, std::move(func));
        }
    }

    template <class Func, class Tuple>
    constexpr void for_each(const Tuple& tuple, Func&& func) {
        For_each_impl<Func, Tuple, 0>(tuple, std::move(func));
    }
}

#ifndef ENFORCE_BAN_OF_REFL_NAMESPACE
namespace refl = reflection;
#endif // ENFORCE_BAN_OF_REFL_NAMESPACE

#endif // REFLECTION_BASE_HPP