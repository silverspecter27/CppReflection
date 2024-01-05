#pragma once
#ifndef REFLECTION_FIELD_HPP
#define REFLECTION_FIELD_HPP
#include "ReflectionBase.hpp"

#include <tuple>

__REFLECTION_BEGIN__
template <class Object, class Ty>
struct Field {
public:
    using object_type = Object;
    using value_type  = Ty;

    constexpr Field()                        noexcept = default;
    constexpr Field(const Field&)            noexcept = default;
    constexpr Field& operator=(const Field&) noexcept = default;

    _NODISCARD constexpr const Class<Object>& get_declared_class() const noexcept {
        return get_class<Object>();
    }

    _NODISCARD inline std::size_t hash_code() const noexcept {
        return typeid(Field).hash_code();
    }

    virtual _NODISCARD _CONSTEXPR20 Ty& get(nullptr_t) const = 0;

    virtual _NODISCARD _CONSTEXPR20 Ty& get(Object& object) const = 0;

    virtual _NODISCARD _CONSTEXPR20 const Ty& get(const Object& object) const = 0;

    virtual _NODISCARD _CONSTEXPR20 const char* name() const noexcept = 0;
};

#if HAS_CONSTEXPR_STRING
template <cstd::constexpr_string Name, class Object, class Ty, class Tag>
#else /* ^^^ HAS_CONSTEXPR_STRING ^^^ / vvv HAS_CONSTEXPR_STRING vvv */
template <class Object, class Ty, class Tag>
#endif // HAS_CONSTEXPR_STRING
struct Field_impl_ : public Field<Object, Ty> {
private:
    using Mybase = Field<Object, Ty>;

public:
    using object_type = typename Mybase::object_type;
    using value_type  = typename Mybase::value_type;

    constexpr Field_impl_()                              noexcept = default;
    constexpr Field_impl_(const Field_impl_&)            noexcept = default;
    constexpr Field_impl_& operator=(const Field_impl_&) noexcept = default;

    _NODISCARD constexpr Ty& get(nullptr_t) const {
        // You cannot use a nullptr to access a non static field.
        return get(nullptr);
    }

    _NODISCARD constexpr Ty& get(Object& object) const {
        return object.*Get(_tag);
    }

    _NODISCARD constexpr const Ty& get(const Object& object) const {
        return object.*Get(_tag);
    }

    _NODISCARD _CONSTEXPR20 const char* name() const noexcept {
#if HAS_CONSTEXPR_STRING
        return Name.data();
#else /* ^^^ HAS_CONSTEXPR_STRING ^^^ / vvv HAS_CONSTEXPR_STRING vvv */
        return _name;
#endif // HAS_CONSTEXPR_STRING
    }

private:
    static constexpr Tag _tag{};

#if !HAS_CONSTEXPR_STRING
    static const char* _name;
#endif // HAS_CONSTEXPR_STRING
};

#if HAS_CONSTEXPR_STRING
template <cstd::constexpr_string Name, class Object, class Ty, class Tag>
#else /* ^^^ HAS_CONSTEXPR_STRING ^^^ / vvv HAS_CONSTEXPR_STRING vvv */
template <class Object, class Ty, class Tag>
#endif // HAS_CONSTEXPR_STRING
struct StaticField_impl_ : public Field<Object, Ty> {
private:
    using Mybase = Field<Object, Ty>;

public:
    using object_type = typename Mybase::object_type;
    using value_type  = typename Mybase::value_type;

    constexpr StaticField_impl_()                                    = default;
    constexpr StaticField_impl_(const StaticField_impl_&)            = default;
    constexpr StaticField_impl_& operator=(const StaticField_impl_&) = default;

    _NODISCARD constexpr Ty& get(nullptr_t) const {
        return *Get(_tag);
    }

    _NODISCARD constexpr Ty& get(Object& object) const {
        return *Get(_tag);
    }

    _NODISCARD constexpr const Ty& get(const Object& object) const {
        return *Get(_tag);
    }

    _NODISCARD _CONSTEXPR20 const char* name() const noexcept {
#if HAS_CONSTEXPR_STRING
        return Name.data();
#else /* ^^^ HAS_CONSTEXPR_STRING ^^^ / vvv HAS_CONSTEXPR_STRING vvv */
        return _name;
#endif // HAS_CONSTEXPR_STRING
    }

private:
    static constexpr Tag _tag{};

#if !HAS_CONSTEXPR_STRING
    static const char* _name;
#endif // HAS_CONSTEXPR_STRING
};

template <class Object, class Tag>
struct Get_field_by_tag_;

template <class Object, class Tag>
using Get_field_by_tag_t_ = typename Get_field_by_tag_<Object, Tag>::type;

#if HAS_CONSTEXPR_STRING
template <class Object, cstd::constexpr_string Name>
struct Get_field_;

template <class Object, cstd::constexpr_string Name>
using Get_field_t_ = typename Get_field_<Object, Name>::type;

#define DEFINE_FIELD(Type, Name) Field_impl_<#Name, CONCATENATE(Class_, __LINE__), Type, allow_access::Get_field_tag_<CONCATENATE(Class_, __LINE__), tag::Name>>
#define DEFINE_STATIC_FIELD(Type, Name) StaticField_impl_<#Name, CONCATENATE(Class_, __LINE__), Type, allow_access::Get_field_tag_<CONCATENATE(Class_, __LINE__), tag::Name>>

#define DEFINE_GET_FIELD(Type, Name)                      \
template<>                                                \
struct Get_field_<CONCATENATE(Class_, __LINE__), #Name> { \
    using type = DEFINE_FIELD(Type, Name);                \
};

#define DEFINE_GET_STATIC_FIELD(Type, Name)               \
template<>                                                \
struct Get_field_<CONCATENATE(Class_, __LINE__), #Name> { \
    using type = DEFINE_STATIC_FIELD(Type, Name);         \
};
#else /* ^^^ HAS_CONSTEXPR_STRING ^^^ / vvv HAS_CONSTEXPR_STRING vvv */
#define DEFINE_FIELD(Type, Name) Field_impl_<CONCATENATE(Class_, __LINE__), Type, allow_access::Get_field_tag_<CONCATENATE(Class_, __LINE__), tag::Name>>
#define DEFINE_STATIC_FIELD(Type, Name) StaticField_impl_<CONCATENATE(Class_, __LINE__), Type, allow_access::Get_field_tag_<CONCATENATE(Class_, __LINE__), tag::Name>>

#define DEFINE_GET_FIELD(Type, Name) const char* DEFINE_FIELD(Type, Name)::_name = #Name;
#define DEFINE_GET_STATIC_FIELD(Type, Name) const char* DEFINE_STATIC_FIELD(Type, Name)::_name = #Name;
#endif // HAS_CONSTEXPR_STRING

#ifdef __COUNTER__
#define HAS_GET_FIELDS 1
#else /* ^^^ __COUNTER__ ^^^ / vvv !__COUNTER__ vvv */
#define HAS_GET_FIELDS 0
#endif // __COUNTER__

#if HAS_GET_FIELDS
template <class Object>
struct Get_start_field_index_;

template <class Object>
constexpr std::size_t Get_start_field_index_v_ = Get_start_field_index_<Object>::value;

template <class Object>
struct Get_fields_amount_;

template <class Object>
constexpr std::size_t Get_fields_amount_v_ = Get_fields_amount_<Object>::value;

template <class Object, std::size_t Index>
struct Get_field_type_with_index_;

template <class Object, std::size_t Index>
using Get_field_type_with_index_t_ = typename Get_field_type_with_index_<Object, Index>::type;

template <class Object, std::size_t... Is>
_NODISCARD constexpr std::tuple<Get_field_type_with_index_t_<Object, Is + Get_start_field_index_v_<Object>>...> Get_fields_helper_(std::index_sequence<Is...>) {
    return {};
}

template <class Object>
struct Get_fields_ {
    using type = decltype(Get_fields_helper_<Object>(std::make_index_sequence<Get_fields_amount_v_<Object>>{}));
};

template <class Object>
using Get_fields_t_ = typename Get_fields_<Object>::type;

template <class Object>
struct Field_counter_tag_;

template <class Object>
struct Fields_counter_ : cstd::counter<Field_counter_tag_<Object>> {};

#define DECLARE_FIELDS(...)                                                                                                                                             \
template <>                                                                                                                                                             \
struct Get_start_field_index_<CONCATENATE(Class_, __LINE__)> {                                                                                                          \
    static constexpr std::size_t value = Fields_counter_<CONCATENATE(Class_, __LINE__)>::next<__COUNTER__>() + 1;                                                       \
};                                                                                                                                                                      \
                                                                                                                                                                        \
__VA_ARGS__                                                                                                                                                             \
                                                                                                                                                                        \
template <>                                                                                                                                                             \
struct Get_fields_amount_<CONCATENATE(Class_, __LINE__)> {                                                                                                              \
    static constexpr std::size_t value = Fields_counter_<CONCATENATE(Class_, __LINE__)>::next<__COUNTER__>() - Get_start_field_index_v_<CONCATENATE(Class_, __LINE__)>; \
};

#define BEGIN_FIELDS                                                                                                                                                    \
template <>                                                                                                                                                             \
struct Get_start_field_index_<CONCATENATE(Class_, __LINE__)> {                                                                                                          \
    static constexpr std::size_t value = Fields_counter_<CONCATENATE(Class_, __LINE__)>::next<__COUNTER__>() + 1;                                                       \
};                                                                                                                                                                      \

#define END_FIELDS                                                                                                                                                      \
template <>                                                                                                                                                             \
struct Get_fields_amount_<CONCATENATE(Class_, __LINE__)> {                                                                                                              \
    static constexpr std::size_t value = Fields_counter_<CONCATENATE(Class_, __LINE__)>::next<__COUNTER__>() - Get_start_field_index_v_<CONCATENATE(Class_, __LINE__)>; \
};

#define GENERATE_FIELD_WITH_INDEX(Type, Name)                                                                                           \
template <>                                                                                                                             \
struct Get_field_type_with_index_<CONCATENATE(Class_, __LINE__), Fields_counter_<CONCATENATE(Class_, __LINE__)>::next<__COUNTER__>()> { \
    using type = DEFINE_FIELD(Type, Name);                                                                                              \
};

#define GENERATE_STATIC_FIELD_WITH_INDEX(Type, Name)                                                                                    \
template <>                                                                                                                             \
struct Get_field_type_with_index_<CONCATENATE(Class_, __LINE__), Fields_counter_<CONCATENATE(Class_, __LINE__)>::next<__COUNTER__>()> { \
    using type = DEFINE_STATIC_FIELD(Type, Name);                                                                                       \
};
#else /* ^^^ HAS_GET_FIELDS ^^^ / vvv !HAS_GET_FIELDS vvv */
#define DECLARE_FIELDS(...) __VA_ARGS__
#define BEGIN_FIELDS
#define END_FIELDS

#define GENERATE_FIELD_WITH_INDEX(Type, Name)
#define GENERATE_STATIC_FIELD_WITH_INDEX(Type, Name)
#endif // HAS_GET_FIELDS

namespace allow_access {
    template <class Object, class Tag>
    struct Get_field_tag_ {
    };
}

#define FIELD(Type, Name)                                                                                          \
namespace tag {                                                                                                    \
    struct Name;                                                                                                   \
}                                                                                                                  \
                                                                                                                   \
namespace allow_access {                                                                                           \
    template struct private_access<Type(CONCATENATE(Class_, __LINE__)::*),                                         \
        &CONCATENATE(Class_, __LINE__)::Name, Get_field_tag_<CONCATENATE(Class_, __LINE__), tag::Name>>;           \
                                                                                                                   \
    constexpr Type CONCATENATE(Class_, __LINE__)::* Get(Get_field_tag_<CONCATENATE(Class_, __LINE__), tag::Name>); \
}                                                                                                                  \
                                                                                                                   \
template <>                                                                                                        \
struct Get_field_by_tag_<CONCATENATE(Class_, __LINE__), tag::Name> {                                               \
    using type = DEFINE_FIELD(Type, Name);                                                                         \
};                                                                                                                 \
                                                                                                                   \
DEFINE_GET_FIELD(Type, Name)                                                                                       \
                                                                                                                   \
GENERATE_FIELD_WITH_INDEX(Type, Name)

#define STATIC_FIELD(Type, Name)                                                                         \
namespace tag {                                                                                          \
    struct Name;                                                                                         \
}                                                                                                        \
                                                                                                         \
namespace allow_access {                                                                                 \
    template struct private_access<Type(*),                                                              \
        &CONCATENATE(Class_, __LINE__)::Name, Get_field_tag_<CONCATENATE(Class_, __LINE__), tag::Name>>; \
                                                                                                         \
    constexpr Type* Get(Get_field_tag_<CONCATENATE(Class_, __LINE__), tag::Name>);                       \
}                                                                                                        \
                                                                                                         \
template <>                                                                                              \
struct Get_field_by_tag_<CONCATENATE(Class_, __LINE__), tag::Name> {                                     \
    using type = DEFINE_STATIC_FIELD(Type, Name);                                                        \
};                                                                                                       \
                                                                                                         \
DEFINE_GET_STATIC_FIELD(Type, Name)                                                                      \
                                                                                                         \
GENERATE_STATIC_FIELD_WITH_INDEX(Type, Name)
__REFLECTION_END__

#endif // REFLECTION_FIELD_HPP