#pragma once
#ifndef REFLECTION_METHOD_HPP
#define REFLECTION_METHOD_HPP
#include "ReflectionBase.hpp"

#include <tuple>

__REFLECTION_BEGIN__
template <class Object, class Ret, class... Args>
struct Method {
public:
    using object_type = Object;
    using return_type = Ret;

    constexpr Method()                         noexcept = default;
    constexpr Method(const Method&)            noexcept = default;
    constexpr Method& operator=(const Method&) noexcept = default;

public:
    _NODISCARD constexpr const Class<Object>& get_declared_class() const noexcept {
        return get_class<Object>();
    }

    _NODISCARD inline std::size_t hash_code() const noexcept {
        return typeid(Method).hash_code();
    }

    constexpr Ret operator()(nullptr_t, Args... args) const {
        return invoke(nullptr, args...);
    }

    constexpr Ret operator()(Object& object, Args... args) const {
        return invoke(object, args...);
    }

    virtual _CONSTEXPR20 Ret invoke(nullptr_t, Args... args) const = 0;

    virtual _CONSTEXPR20 Ret invoke(Object& object, Args... args) const = 0;

    virtual _NODISCARD _CONSTEXPR20 const char* name() const noexcept = 0;
};

template <class Object, class Tag, class Ret, class... Args>
struct Method_class_ : public Method<Object, Ret, Args...> {
private:
    using Mybase = Method<Object, Ret, Args...>;

public:
    using object_type = typename Mybase::object_type;
    using return_type = typename Mybase::return_type;

    constexpr Method_class_()                                noexcept = default;
    constexpr Method_class_(const Method_class_&)            noexcept = default;
    constexpr Method_class_& operator=(const Method_class_&) noexcept = default;

public:
    constexpr Ret invoke(nullptr_t, Args... args) const {
        // You cannot use a nullptr to invoke a non static method.
        return invoke(nullptr, args...);
    }

    constexpr Ret invoke(Object& object, Args... args) const {
        return (object.*Get(_tag))(args...);
    }

private:
    static constexpr Tag _tag{};
};

template <class Object, class Tag, class Tx>
struct Get_Method_impl_ {
    static_assert(Always_false<Tx>, "reflection::Method only accepts method types as template arguments.");
};

template <class Object, class Tag, class Ret, class... Args>
struct Get_Method_impl_<Object, Tag, Ret(Args...)> {
    using type = Method_class_<Object, Tag, Ret, Args...>;
};

#if HAS_CONSTEXPR_STRING
template <cstd::constexpr_string Name, class Object, class Ty, class Tag>
#else /* ^^^ HAS_CONSTEXPR_STRING ^^^ / vvv HAS_CONSTEXPR_STRING vvv */
template <class Object, class Ty, class Tag>
#endif // HAS_CONSTEXPR_STRING
struct Method_impl_ : public Get_Method_impl_<Object, Tag, Ty>::type {
private:
    using Mybase = typename Get_Method_impl_<Object, Tag, Ty>::type;

public:
    using object_type = typename Mybase::object_type;
    using return_type = typename Mybase::return_type;

    constexpr Method_impl_()                               noexcept = default;
    constexpr Method_impl_(const Method_impl_&)            noexcept = default;
    constexpr Method_impl_& operator=(const Method_impl_&) noexcept = default;

public:
    _NODISCARD _CONSTEXPR20 const char* name() const noexcept {
#if HAS_CONSTEXPR_STRING
        return Name.data();
#else /* ^^^ HAS_CONSTEXPR_STRING ^^^ / vvv HAS_CONSTEXPR_STRING vvv */
        return _name;
#endif // HAS_CONSTEXPR_STRING
    }

#if !HAS_CONSTEXPR_STRING
private:
    static const char* _name;
#endif // HAS_CONSTEXPR_STRING
};

template <class Object, class Tag, class Ret, class... Args>
struct StaticMethod_class_ : public Method<Object, Ret, Args...> {
private:
    using Mybase = Method<Object, Ret, Args...>;

public:
    using object_type = typename Mybase::object_type;
    using return_type = typename Mybase::return_type;

    constexpr StaticMethod_class_()                                      noexcept = default;
    constexpr StaticMethod_class_(const StaticMethod_class_&)            noexcept = default;
    constexpr StaticMethod_class_& operator=(const StaticMethod_class_&) noexcept = default;

public:
    constexpr Ret invoke(nullptr_t, Args... args) const {
        return (*Get(_tag))(args...);
    }

    constexpr Ret invoke(Object&, Args... args) const {
        return (*Get(_tag))(args...);
    }

private:
    static constexpr Tag _tag{};
};

template <class Object, class Tag, class Tx>
struct Get_StaticMethod_impl_ {
    static_assert(Always_false<Tx>, "reflection::Method only accepts method types as template arguments.");
};

template <class Object, class Tag, class Ret, class... Args>
struct Get_StaticMethod_impl_<Object, Tag, Ret(Args...)> {
    using type = StaticMethod_class_<Object, Tag, Ret, Args...>;
};

#if HAS_CONSTEXPR_STRING
template <cstd::constexpr_string Name, class Object, class Ty, class Tag>
#else /* ^^^ HAS_CONSTEXPR_STRING ^^^ / vvv HAS_CONSTEXPR_STRING vvv */
template <class Object, class Ty, class Tag>
#endif // HAS_CONSTEXPR_STRING
struct StaticMethod_impl_ : public Get_StaticMethod_impl_<Object, Tag, Ty>::type {
private:
    using Mybase = typename Get_StaticMethod_impl_<Object, Tag, Ty>::type;

public:
    using object_type = typename Mybase::object_type;
    using return_type = typename Mybase::return_type;

    constexpr StaticMethod_impl_()                                     noexcept = default;
    constexpr StaticMethod_impl_(const StaticMethod_impl_&)            noexcept = default;
    constexpr StaticMethod_impl_& operator=(const StaticMethod_impl_&) noexcept = default;

public:
    _NODISCARD _CONSTEXPR20 const char* name() const noexcept {
#if HAS_CONSTEXPR_STRING
        return Name.data();
#else /* ^^^ HAS_CONSTEXPR_STRING ^^^ / vvv HAS_CONSTEXPR_STRING vvv */
        return _name;
#endif // HAS_CONSTEXPR_STRING
    }

#if !HAS_CONSTEXPR_STRING
private:
    static const char* _name;
#endif // HAS_CONSTEXPR_STRING
};

template <class Ty>
struct Get_method_args_;

template <class Ret, class... Args>
struct Get_method_args_<Ret(Args...)> {
    using type = std::tuple<Args...>;
};

template <class Ty>
using Get_method_args_t_ = typename Get_method_args_<Ty>::type;

template <class Object, class Tag, class Args>
struct Get_method_by_tag_;

template <class Object, class Tag, class Args>
using Get_method_by_tag_t_ = typename Get_method_by_tag_<Object, Tag, Args>::type;

#if HAS_CONSTEXPR_STRING
template <class Object, cstd::constexpr_string Name, class Args>
struct Get_method_;

template <class Object, cstd::constexpr_string Name, class Args>
using Get_method_t_ = typename Get_method_<Object, Name, Args>::type;

#define DEFINE_METHOD(Type, Name) Method_impl_<#Name, CONCATENATE(Class_, __LINE__), Type, allow_access::Get_method_tag_<CONCATENATE(Class_, __LINE__), tag::Name, Get_method_args_t_<Type>>>
#define DEFINE_STATIC_METHOD(Type, Name) StaticMethod_impl_<#Name, CONCATENATE(Class_, __LINE__), Type, allow_access::Get_method_tag_<CONCATENATE(Class_, __LINE__), tag::Name, Get_method_args_t_<Type>>>

#define DEFINE_GET_METHOD(Type, Name)                                                \
template<>                                                                           \
struct Get_method_<CONCATENATE(Class_, __LINE__), #Name, Get_method_args_t_<Type>> { \
    using type = DEFINE_METHOD(Type, Name);                                          \
};

#define DEFINE_GET_STATIC_METHOD(Type, Name)                                         \
template<>                                                                           \
struct Get_method_<CONCATENATE(Class_, __LINE__), #Name, Get_method_args_t_<Type>> { \
    using type = DEFINE_STATIC_METHOD(Type, Name);                                   \
};
#else /* ^^^ HAS_CONSTEXPR_STRING ^^^ / vvv HAS_CONSTEXPR_STRING vvv */
#define DEFINE_METHOD(Type, Name) Method_impl_<CONCATENATE(Class_, __LINE__), Type, allow_access::Get_method_tag_<CONCATENATE(Class_, __LINE__), Name, Get_method_args_t_<Type>>>
#define DEFINE_STATIC_METHOD(Type, Name) StaticMethod_impl_<CONCATENATE(Class_, __LINE__), Type, allow_access::Get_method_tag_<CONCATENATE(Class_, __LINE__), Name, Get_method_args_t_<Type>>>

#define DEFINE_GET_METHOD(Type, Name) const char* DEFINE_METHOD(Type, Name)::_name = #Name;
#define DEFINE_GET_STATIC_METHOD(Type, Name) const char* DEFINE_STATIC_METHOD(Type, Name)::_name = #Name;
#endif // HAS_CONSTEXPR_STRING

#ifdef __COUNTER__
#define HAS_GET_METHODS 1
#else /* ^^^ __COUNTER__ ^^^ / vvv !__COUNTER__ vvv */
#define HAS_GET_METHODS 0
#endif // __COUNTER__

#if HAS_GET_METHODS
template <class Object>
struct Get_start_method_index_;

template <class Object>
constexpr std::size_t Get_start_method_index_v_ = Get_start_method_index_<Object>::value;

template <class Object>
struct Get_methods_amount_;

template <class Object>
constexpr std::size_t Get_methods_amount_v_ = Get_methods_amount_<Object>::value;

template <class Object, std::size_t Index>
struct Get_method_type_with_index_;

template <class Object, std::size_t Index>
using Get_method_type_with_index_t_ = typename Get_method_type_with_index_<Object, Index>::type;

template <class Object, std::size_t... Is>
_NODISCARD constexpr std::tuple<Get_method_type_with_index_t_<Object, Is + Get_start_method_index_v_<Object>>...> Get_methods_helper_(std::index_sequence<Is...>) {
    return {};
}

template <class Object>
struct Get_methods_ {
    using type = decltype(Get_methods_helper_<Object>(std::make_index_sequence<Get_methods_amount_v_<Object>>{}));
};

template <class Object>
using Get_methods_t_ = typename Get_methods_<Object>::type;

template <class Object>
struct Method_counter_tag_;

template <class Object>
struct Methods_counter_ : cstd::counter<Method_counter_tag_<Object>> {};

#define DECLARE_METHODS(...)                                                                                                                                              \
template <>                                                                                                                                                               \
struct Get_start_method_index_<CONCATENATE(Class_, __LINE__)> {                                                                                                           \
    static constexpr std::size_t value = Methods_counter_<CONCATENATE(Class_, __LINE__)>::next<__COUNTER__>() + 1;                                                        \
};                                                                                                                                                                        \
                                                                                                                                                                          \
__VA_ARGS__                                                                                                                                                               \
                                                                                                                                                                          \
template <>                                                                                                                                                               \
struct Get_methods_amount_<CONCATENATE(Class_, __LINE__)> {                                                                                                               \
    static constexpr std::size_t value = Methods_counter_<CONCATENATE(Class_, __LINE__)>::next<__COUNTER__>() - Get_start_method_index_v_<CONCATENATE(Class_, __LINE__)>; \
};

#define BEGIN_METHODS                                                                                              \
template <>                                                                                                        \
struct Get_start_method_index_<CONCATENATE(Class_, __LINE__)> {                                                    \
    static constexpr std::size_t value = Methods_counter_<CONCATENATE(Class_, __LINE__)>::next<__COUNTER__>() + 1; \
};

#define END_METHODS                                                                                                                                                       \
template <>                                                                                                                                                               \
struct Get_methods_amount_<CONCATENATE(Class_, __LINE__)> {                                                                                                               \
    static constexpr std::size_t value = Methods_counter_<CONCATENATE(Class_, __LINE__)>::next<__COUNTER__>() - Get_start_method_index_v_<CONCATENATE(Class_, __LINE__)>; \
};

#define GENERATE_METHOD_WITH_INDEX(Type, Name)                                                                                            \
template <>                                                                                                                               \
struct Get_method_type_with_index_<CONCATENATE(Class_, __LINE__), Methods_counter_<CONCATENATE(Class_, __LINE__)>::next<__COUNTER__>()> { \
    using type = DEFINE_METHOD(Type, Name);                                                                                               \
};

#define GENERATE_STATIC_METHOD_WITH_INDEX(Type, Name)                                                                                     \
template <>                                                                                                                               \
struct Get_method_type_with_index_<CONCATENATE(Class_, __LINE__), Methods_counter_<CONCATENATE(Class_, __LINE__)>::next<__COUNTER__>()> { \
    using type = DEFINE_STATIC_METHOD(Type, Name);                                                                                        \
};
#else /* ^^^ HAS_GET_METHODS ^^^ / vvv !HAS_GET_METHODS vvv */
#define DECLARE_METHODS(...) __VA_ARGS__
#define BEGIN_METHODS
#define END_METHODS

#define GENERATE_METHOD_WITH_INDEX(Type, Name)
#define GENERATE_STATIC_METHOD_WITH_INDEX(Type, Name)
#endif // HAS_GET_METHODS

template <class Ty>
using Method_type_t_ = Ty;

namespace allow_access {
    template <class Object, class Tag, class Args>
    struct Get_method_tag_ {
    };
}

#define METHOD(Type, Name)                                                                                                          \
namespace tag {                                                                                                                     \
    struct Name;                                                                                                                    \
}                                                                                                                                   \
                                                                                                                                    \
namespace allow_access {                                                                                                            \
    template struct private_access<Method_type_t_<Type>(CONCATENATE(Class_, __LINE__)::*),                                          \
        &CONCATENATE(Class_, __LINE__)::Name, Get_method_tag_<CONCATENATE(Class_, __LINE__), tag::Name, Get_method_args_t_<Type>>>; \
                                                                                                                                    \
    constexpr Method_type_t_<Type> CONCATENATE(Class_, __LINE__)::*                                                                 \
        Get(Get_method_tag_<CONCATENATE(Class_, __LINE__), tag::Name, Get_method_args_t_<Type>>);                                   \
}                                                                                                                                   \
                                                                                                                                    \
template <>                                                                                                                         \
struct Get_method_by_tag_<CONCATENATE(Class_, __LINE__), tag::Name, Get_method_args_t_<Type>> {                                     \
    using type = DEFINE_METHOD(Type, Name);                                                                                         \
};                                                                                                                                  \
                                                                                                                                    \
DEFINE_GET_METHOD(Type, Name)                                                                                                       \
                                                                                                                                    \
GENERATE_METHOD_WITH_INDEX(Type, Name)

#define STATIC_METHOD(Type, Name)                                                                                                   \
namespace tag {                                                                                                                     \
    struct Name;                                                                                                                    \
}                                                                                                                                   \
                                                                                                                                    \
namespace allow_access {                                                                                                            \
    template struct private_access<Method_type_t_<Type>(*),                                                                         \
        &CONCATENATE(Class_, __LINE__)::Name, Get_method_tag_<CONCATENATE(Class_, __LINE__), tag::Name, Get_method_args_t_<Type>>>; \
                                                                                                                                    \
    constexpr Method_type_t_<Type>*                                                                                                 \
        Get(Get_method_tag_<CONCATENATE(Class_, __LINE__), tag::Name, Get_method_args_t_<Type>>);                                   \
}                                                                                                                                   \
                                                                                                                                    \
template <>                                                                                                                         \
struct Get_method_by_tag_<CONCATENATE(Class_, __LINE__), tag::Name, Get_method_args_t_<Type>> {                                     \
    using type = DEFINE_STATIC_METHOD(Type, Name);                                                                                  \
};                                                                                                                                  \
                                                                                                                                    \
DEFINE_GET_STATIC_METHOD(Type, Name)                                                                                                \
                                                                                                                                    \
GENERATE_STATIC_METHOD_WITH_INDEX(Type, Name)
__REFLECTION_END__

#endif // REFLECTION_METHOD_HPP