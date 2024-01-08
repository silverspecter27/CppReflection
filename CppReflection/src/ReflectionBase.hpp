#pragma once
#ifndef REFLECTION_BASE_HPP
#define REFLECTION_BASE_HPP
#include "vals_core.hpp"

#include <utility>

#include "constexpr_string.hpp"
#include "counter.hpp"

#define __REFLECTION_BEGIN__ namespace reflection {
#define __REFLECTION_END__   }
#define __REFLECTION__       ::reflection::

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

template <class Ty, class... Args>
struct Static_base_ {
    virtual _NODISCARD _CONSTEXPR20 Ty operator()(nullptr_t, Args...) const = 0;
};

template <class Object>
struct Class;

template <class Object>
struct Raw_object_;

template <class Object>
using Raw_object_t_ = typename Raw_object_<Object>::type;

template <class Object>
_NODISCARD constexpr const Class<Raw_object_t_<Object>> get_class() noexcept;
__REFLECTION_END__

namespace cstd {
#if 1 // CSTD_BINARY_LOOKUP
    template <class Func, class Tuple, std::size_t Index>
    constexpr void For_each_impl(const Tuple& tuple, Func&& func) {
        (void)func(std::get<Index>(tuple));

        if constexpr (Index + 1 < std::tuple_size_v<Tuple>) {
            For_each_impl<Func, Tuple, Index + 1>(tuple, std::move(func));
        }
    }
#else /* ^^^ CSTD_BINARY_LOOKUP ^^^ / vvv !CSTD_BINARY_LOOKUP vvv */
    template <class Func, class Tuple, std::size_t Index, bool Stop>
    struct For_each_impl_;

    template <class Func, class Tuple, std::size_t Index>
    struct For_each_impl_<Func, Tuple, Index, false> {
        static constexpr void operate(const Tuple& tuple, Func&& func) {
        }
    };

    template <class Func, class Tuple, std::size_t Index>
    struct For_each_impl_<Func, Tuple, Index, true> {
        static constexpr void operate(const Tuple& tuple, Func&& func) {
            (void)func(std::get<Index>(tuple));
            For_each_impl_ < Func, Tuple, Index + 1, Index + 1 < std::tuple_size_v<Tuple> > ::operate(tuple, std::move(func));
        }
    };

    template <class Func, class Tuple, std::size_t Index>
    constexpr void For_each_impl(const Tuple& tuple, Func&& func) {
        For_each_impl_ < Func, Tuple, 0, 1 < std::tuple_size_v<Tuple> > ::operate(tuple, std::move(func));
    }
#endif // CSTD_BINARY_LOOKUP

    template <class Func, class Tuple>
    constexpr void for_each(const Tuple& tuple, Func&& func) {
        For_each_impl<Func, Tuple, 0>(tuple, std::move(func));
    }
}

#ifndef ENFORCE_BAN_OF_REFL_NAMESPACE
namespace refl = reflection;
#endif // ENFORCE_BAN_OF_REFL_NAMESPACE

#endif // REFLECTION_BASE_HPP