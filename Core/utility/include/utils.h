﻿// Created by BlurringShadow at 2021-02-27-下午 10:24

#pragma once

#include <concepts>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>

#include <nameof.hpp>
#include <entt/entt.hpp>
#include <fmt/ostream.h>
#include <frozen/set.h>
#include <frozen/string.h>
#include <frozen/unordered_map.h>
#include <frozen/unordered_set.h>
#include <range/v3/all.hpp>
#include <rxcpp/rx.hpp>
#include <tl/expected.hpp>

namespace polycumic::utility
{
    using tl::expected;

    template<typename T>
    struct auto_cast
    {
        T&& t;

        template<typename U> requires requires { static_cast<std::decay_t<U>>(std::forward<T>(t)); }
        [[nodiscard]] constexpr operator
        U() noexcept(std::is_nothrow_constructible_v<T, std::decay_t<U>>)
        {
            return static_cast<std::decay_t<U>>(std::forward<T>(t));
        }
    };

    template<typename T>
    auto_cast(T&& t) -> auto_cast<T>;

    template<typename T, typename U, typename Comp>
    constexpr T& set_if(T& left, U&& right, Comp comp)
    {
        if(comp(right, left)) left = std::forward<U>(right);
        return left;
    }

    template<typename T, typename U>
    constexpr T& set_if_greater(T& left, U&& right)
    {
        return set_if(left, std::forward<U>(right), std::greater<>{});
    }

    template<typename T, typename U>
    constexpr T& set_if_lesser(T& left, U&& right)
    {
        return set_if(left, std::forward<U>(right), std::less<>{});
    }

    template<typename T, typename Compare>
    [[nodiscard]] constexpr bool is_between(const T& v, const T& min, const T& max, Compare cmp)
    {
        return std::addressof(std::clamp(v, min, max, cmp)) == std::addressof(v);
    }

    template<typename T>
    [[nodiscard]] constexpr bool is_between(const T& v, const T& min, const T& max)
    {
        return is_between(v, min, max, std::less<>{});
    }

#define AUTO_MEMBER(name, ini) decltype(ini) name = ini

#define AUTO_RVALUE_MEMBER(name, ini) decltype(ini)&& name = ini

#define CONST_AUTO_MEMBER(name, ini) const decltype(ini)& name = ini
}