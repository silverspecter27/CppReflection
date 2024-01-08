#pragma once
#ifndef REFLECTION_CLASS_HPP
#define REFLECTION_CLASS_HPP
#include "ReflectionBase.hpp"

#include "Field.hpp"
#include "Method.hpp"

__REFLECTION_BEGIN__
template <class Object>
struct Class final {
    template <class UObject>
    friend constexpr const Class<Raw_object_t_<UObject>> get_class() noexcept;

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

    template <class Tag, class... Args>
    _NODISCARD constexpr Get_method_by_tag_t_<Object, Tag, std::tuple<Args...>> get_method() const {
        return {};
    }

#if HAS_CONSTEXPR_STRING
    template <cstd::constexpr_string Name, class... Args>
    _NODISCARD constexpr Get_method_t_<Object, Name, std::tuple<Args...>> get_method() const {
        return {};
    }
#endif // HAS_CONSTEXPR_STRING

#if HAS_GET_METHODS
    _NODISCARD constexpr Get_methods_t_<Object> get_methods() const {
        return {};
    }
#endif // HAS_GET_METHODS

    _NODISCARD constexpr bool is_array() const noexcept {
        return std::is_array_v<Object>;
    }

    _NODISCARD constexpr bool is_enum() const noexcept {
        return std::is_enum_v<Object>;
    }

    _NODISCARD constexpr bool is_arithmetic() const noexcept {
        return std::is_arithmetic_v<Object>;
    }

    _NODISCARD constexpr bool is_pointer() const noexcept {
        return std::is_pointer_v<Object>;
    }

    _NODISCARD constexpr bool is_primitive() const noexcept {
        return std::is_fundamental_v<Object>;
    }

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
struct Raw_object_ {
    using type = std::remove_volatile_t<std::remove_const_t<std::remove_reference_t<Object>>>;
};

template <class Object>
_NODISCARD constexpr const Class<Raw_object_t_<Object>> get_class() noexcept {
    return {};
}
__REFLECTION_END__

#define REFLECT(Object, ...)                                 \
__REFLECTION_BEGIN__                                         \
using CONCATENATE(Class_, __LINE__) = Raw_object_t_<Object>; \
                                                             \
__VA_ARGS__                                                  \
__REFLECTION_END__

#endif // REFLECTION_CLASS_HPP