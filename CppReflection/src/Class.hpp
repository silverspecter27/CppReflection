#pragma once
#ifndef REFLECTION_CLASS_HPP
#define REFLECTION_CLASS_HPP
#include "ReflectionBase.hpp"

#include <tuple>

#include "Field.hpp"
#include "Method.hpp"

__REFLECTION_BEGIN__
template <class Object>
struct Class final {
    template <class UObject>
    friend constexpr const Class<UObject> get_class() noexcept;

public:
    using object_type = Object;

private:
    constexpr Class()                        noexcept = default;
    constexpr Class(const Class&)            noexcept = default;
    constexpr Class& operator=(const Class&) noexcept = default;

public:
    template <class Tag>
    _NODISCARD constexpr Get_field_by_tag_t_<Object, Tag> get_field() const {
        return {};
    }

    template <class... Tags>
    _NODISCARD constexpr std::tuple<Get_field_by_tag_t_<Object, Tags>...> get_fields() const {
        return {};
    }

#if HAS_CONSTEXPR_STRING
    template <cstd::constexpr_string Name>
    _NODISCARD constexpr Get_field_t_<Object, Name> get_field() const {
        return {};
    }

    template <cstd::constexpr_string... Names>
    _NODISCARD constexpr std::tuple<Get_field_t_<Object, Names>...> get_fields() const {
        return {};
    }
#endif // HAS_CONSTEXPR_STRING

#if HAS_GET_FIELDS
    _NODISCARD constexpr Get_fields_t_<Object> get_fields() const {
        return {};
    }
#endif // HAS_GET_FIELDS

    template <class Tag>
    _NODISCARD constexpr Get_method_by_tag_t_<Object, Tag> get_method() const {
        return {};
    }

    template <class... Tags>
    _NODISCARD constexpr std::tuple<Get_method_by_tag_t_<Object, Tags>...> get_methods() const {
        return {};
    }

#if HAS_CONSTEXPR_STRING
    template <cstd::constexpr_string Name>
    _NODISCARD constexpr Get_method_t_<Object, Name> get_method() const {
        return {};
    }

    template <cstd::constexpr_string... Names>
    _NODISCARD constexpr std::tuple<Get_method_t_<Object, Names>...> get_methods() const {
        return {};
    }
#endif // HAS_CONSTEXPR_STRING

#if HAS_GET_METHODS
    _NODISCARD constexpr Get_methods_t_<Object> get_methods() const {
        return {};
    }
#endif // HAS_GET_METHODS

    _NODISCARD inline const char* name() const noexcept {
        return typeid(Object).name();
    }

    _NODISCARD inline const char* raw_name() const noexcept {
        return typeid(Object).raw_name();
    }

    _NODISCARD inline std::size_t hash_code() const noexcept {
        return typeid(Object).hash_code();
    }
};

template <class Object>
_NODISCARD constexpr const Class<Object> get_class() noexcept {
    return {};
}
__REFLECTION_END__

#define REFLECT(Object, ...)                  \
__REFLECTION_BEGIN__                          \
using CONCATENATE(Class_, __LINE__) = Object; \
                                              \
__VA_ARGS__                                   \
__REFLECTION_END__

#endif // REFLECTION_CLASS_HPP