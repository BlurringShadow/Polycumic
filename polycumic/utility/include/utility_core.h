﻿// Created by BlurringShadow at 2021-02-27-下午 10:24

#pragma once

#include <algorithm>
#include <chrono>

#ifdef __cpp_lib_concepts
#include <concepts>
#include <ranges>
#endif

#include <execution>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include <nameof.hpp>
#include <boost/type_traits.hpp>
#include <entt/entt.hpp>
#include <fmt/ostream.h>
#include <glm/ext.hpp>
#include <gsl/gsl>
#include <range/v3/all.hpp>
#include <rxcpp/rx.hpp>
#include <tl/expected.hpp>

using namespace std::literals;

namespace polycumic::utility
{
#ifndef __cpp_lib_concepts
#define SFINAE(...) std::enable_if_t<__VA_ARGS__>* = nullptr
#endif
    using tl::expected;

    template<typename T>
    struct auto_cast
    {
        T&& t;

        template<typename U>
#ifdef __cpp_lib_concepts
        requires requires { static_cast<std::decay_t<U>>(std::forward<T>(t)); }
#endif
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
    [[nodiscard]] constexpr bool is_between(
        const T& v,
        const boost::type_identity_t<T>& min,
        const boost::type_identity_t<T>& max,
        Compare cmp
    ) { return std::addressof(std::clamp(v, min, max, cmp)) == std::addressof(v); }

    template<typename T>
    [[nodiscard]] constexpr bool is_between(
        const T& v,
        const boost::type_identity_t<T>& min,
        const boost::type_identity_t<T>& max
    ) { return is_between(v, min, max, std::less<>{}); }

#define AUTO_MEMBER(name, ini) decltype(ini) name = ini

#define CONST_AUTO_MEMBER(name, ini) const decltype(ini) name = ini

#define CONST_AUTO_REF_MEMBER(name, ini) const decltype(ini)& name = ini

    namespace details
    {
        inline static std::random_device random_device;

        inline static std::mt19937 random_engine{std::random_device{}()};
    }

    constexpr auto& get_random_device() { return details::random_device; }

    constexpr auto& get_random_engine() { return details::random_engine; }

    CPP_template(typename T)(requires std::is_enum_v<T>)
    constexpr auto to_underlying(const T v) { return static_cast<std::underlying_type_t<T>>(v); }
}
