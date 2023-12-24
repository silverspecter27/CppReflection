#pragma once
#ifndef CSTD_COUNTER_HPP
#define CSTD_COUNTER_HPP
#include "vals_core.hpp"

#if defined(__GNUC__) && !defined(__clang__)
// There appears to be a bug on gcc that makes it emit a diagnostic that cannot be turned off in certain conditions. This will silence it.
// See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=112267
#define CSTD_FRIEND_RETURN_TYPE auto
#else
#define CSTD_FRIEND_RETURN_TYPE bool
#endif

#if !defined(CSTD_FRIEND_INJECTION_PRAGMA_BEGIN) && !defined(CSTD_FRIEND_INJECTION_PRAGMA_END)
#if defined(__INTEL_COMPILER)
#define CSTD_FRIEND_INJECTION_PRAGMA_BEGIN \
    _Pragma("warning push");               \
    _Pragma("warning disable 1624");

#define CSTD_FRIEND_INJECTION_PRAGMA_END _Pragma("warning pop");
#elif defined(__clang__)
#define CSTD_FRIEND_INJECTION_PRAGMA_BEGIN                      \
    _Pragma("GCC diagnostic push");                             \
    _Pragma("GCC diagnostic ignored \"-Wundefined-internal\"");

#define CSTD_FRIEND_INJECTION_PRAGMA_END _Pragma("GCC diagnostic pop");
#elif defined(__GNUC__)
#define CSTD_FRIEND_INJECTION_PRAGMA_BEGIN                       \
    _Pragma("GCC diagnostic push");                              \ 
    _Pragma("GCC diagnostic ignored \"-Wnon-template-friend\""); \
    _Pragma("GCC diagnostic ignored \"-Wunused-function\"");

#define CSTD_FRIEND_INJECTION_PRAGMA_END _Pragma("GCC diagnostic pop");
#else
#define CSTD_FRIEND_INJECTION_PRAGMA_BEGIN
#define CSTD_FRIEND_INJECTION_PRAGMA_END
#endif
#endif

namespace cstd {
    template <class Context, int Start = 0, int Step = 1>
    class counter {
    public:
        template <class Unique>
        static constexpr int next() {
            return next<Unique>(0) * Step + Start;
        }

        template <int>
        static constexpr int next() {
            struct Unique {};
            return next<Unique>(0) * Step + Start;
        }

        template <class Unique>
        static constexpr int current() {
            return current<Unique>(0) * Step + Start;
        }

        template <int>
        static constexpr int current() {
            struct Unique {};
            return current<Unique>(0) * Step + Start;
        }

    private:
        template <int Value>
        struct slot {
            CSTD_FRIEND_INJECTION_PRAGMA_BEGIN

                friend constexpr CSTD_FRIEND_RETURN_TYPE slot_allocated(slot<Value>);

            CSTD_FRIEND_INJECTION_PRAGMA_END
        };

        template <int UValue>
        struct allocate_slot {
            friend constexpr CSTD_FRIEND_RETURN_TYPE slot_allocated(slot<UValue>) {
                return true;
            }

            enum {
                value = UValue
            };
        };

#if CSTD_BINARY_LOOKUP
        template <class Unique, int Value = 0, int Period = 0, bool = slot_allocated(slot< Value + (1 << Period) - 1>())>
        static constexpr int next(int) {
            return next<Unique, Value, Period + 1>(0);
        }

        template <class Unique, int Value = 0, int Period = 0>
        static constexpr int next(double) {
            if constexpr (Period == 0) {
                return allocate_slot<Value>::value;
            }
            else {
                return next<Unique, Value + (1 << (Period - 1)), 0>(0);
            }
        }

        template <class Unique, int Value = 0, int Period = 0, bool = slot_allocated(slot< Value + (1 << Period) - 1>())>
        static constexpr int current(int) {
            return current<Unique, Value, Period + 1>(0);
        }

        template <class Unique, int Value = 0, int Period = 0>
        static constexpr int current(double) {
            static_assert(Value != 0 || Period != 0, "You must invoke next() first");

            if constexpr (Period == 0) {
                return Value - 1;
            }
            else {
                return current<Unique, Value + (1 << (Period - 1)), 0>(0);
            }
        }
#else /* ^^^ CSTD_BINARY_LOOKUP ^^^ / vvv !CSTD_BINARY_LOOKUP vvv */
        template <class Unique, int Value = 0, bool = slot_allocated(slot<Value>())>
        static constexpr int next(int) {
            return next<Unique, Value + 1>(0);
        }

        template <class Unique, int Value = 0>
        static constexpr int next(double) {
            return allocate_slot<Value>::value;
        }

        template <class Unique, int Value = Start, bool = slot_allocated(slot<Value>())>
        static constexpr int current(int) {
            return current<Unique, Value + 1>(0);
        }

        template <class Unique, int Value = Start>
        static constexpr int current(double) {
            static_assert(Value != 0, "You must invoke next() first");
            return Value - 1;
        }
#endif // CSTD_BINARY_LOOKUP
    };
}
#endif // CSTD_COUNTER_HPP