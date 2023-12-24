#pragma once
#ifndef CSTD_VALS_CORE_HPP
#define CSTD_VALS_CORE_HPP

#define CONCATENATE_IMPL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_IMPL(x, y)

#if !defined(__cpp_if_constexpr) || __cpp_if_constexpr < 201606L
#if defined(CSTD_BINARY_LOOKUP) && CSTD_BINARY_LOOKUP
#error "Binary lookup is only available when compiling with c++17 and above"
#endif

#undef CSTD_BINARY_LOOKUP
#define CSTD_BINARY_LOOKUP 0
#else
#define CSTD_BINARY_LOOKUP 1
#endif

#endif // CSTD_VALS_CORE_HPP