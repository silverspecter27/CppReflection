#pragma once
#ifndef CSTD_CONSTEXPR_STRING_HPP
#define CSTD_CONSTEXPR_STRING_HPP

#if _HAS_CXX20
#define HAS_CONSTEXPR_STRING 1
#else /* ^^^ _HAS_CXX20 ^^^ / vvv !_HAS_CXX20 vvv */
#define HAS_CONSTEXPR_STRING 0
#endif // _HAS_CXX20

#if HAS_CONSTEXPR_STRING
#include <iostream>

namespace cstd {
    template <class Elem, std::size_t Size>
    struct basic_constexpr_string {
        Elem _data[Size]{};

        constexpr basic_constexpr_string(const Elem(&chars)[Size]) {
            std::copy_n(chars, Size, _data);
        }

        constexpr bool operator==(const basic_constexpr_string<Elem, Size> right) const {
            return std::equal(right._data, right._data + Size, _data);
        }

        template<class UElem, std::size_t USize>
        constexpr bool operator==(const basic_constexpr_string<UElem, USize>) const {
            return false;
        }

        template<std::size_t USize>
        constexpr basic_constexpr_string<Elem, Size + USize - 1> operator+(const basic_constexpr_string<Elem, USize> right) const {
            Elem new_char[Size + USize - 1]{};
            std::copy_n(_data, Size - 1, new_char);
            std::copy_n(right._data, USize, new_char + Size - 1);
            return new_char;
        }

        constexpr Elem& at(std::size_t index) const {
            if (Size <= index) {
                std::_Xout_of_range("invalid string position");
            }
            return _data[index];
        }

        constexpr Elem& operator[](std::size_t index) const {
#if _CONTAINER_DEBUG_LEVEL > 0
            _STL_VERIFY(index <= Size, "string subscript out of range");
#endif // _CONTAINER_DEBUG_LEVEL > 0
            return _data[index];
        }

        constexpr std::size_t size() const noexcept {
            return Size - 1;
        }

        constexpr const Elem* data() const noexcept {
            return _data;
        }
    };

    template <std::size_t Size>
    using constexpr_string = basic_constexpr_string<char, Size>;
#ifdef __cpp_lib_char8_t
    template <std::size_t Size>
    using constexpr_u8string = basic_constexpr_string<char8_t, Size>;
#endif // __cpp_lib_char8_t
    template <std::size_t Size>
    using constexpr_u16string = basic_constexpr_string<char16_t, Size>;
    template <std::size_t Size>
    using constexpr_u32string = basic_constexpr_string<char32_t, Size>;
    template <std::size_t Size>
    using constexpr_wstring = basic_constexpr_string<wchar_t, Size>;

    template <class Elem, std::size_t LSize, std::size_t RSize>
    constexpr auto operator+(basic_constexpr_string<Elem, LSize> left, const Elem(&right)[RSize]) {
        return left + constexpr_string<RSize>(right);
    }

    template <class Elem, std::size_t LSize, std::size_t RSize>
    constexpr auto operator+(const Elem(&left)[LSize], basic_constexpr_string<Elem, RSize> right) {
        return constexpr_string<LSize>(left) + right;
    }

    template <class Elem, std::size_t LSize, std::size_t RSize>
    constexpr auto operator==(basic_constexpr_string<Elem, LSize> left, const Elem(&right)[RSize]) {
        return left == constexpr_string<RSize>(right);
    }

    template <class Elem, std::size_t LSize, std::size_t RSize>
    constexpr auto operator==(const Elem(&left)[LSize], basic_constexpr_string<Elem, RSize> right) {
        return constexpr_string<LSize>(left) == right;
    }

    template <class Elem, class Traits, std::size_t Size>
    std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& left, basic_constexpr_string<Elem, Size> right) {
        return left << right._data;
    }
}
#endif // _HAS_CXX20

#endif // CSTD_CONSTEXPR_STRING_HPP