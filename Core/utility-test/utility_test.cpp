﻿// Created by BlurringShadow at 2021-02-28-上午 11:57

#define BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>

#include "reflection.h"

#include "traits/type_container.h"

using namespace polycumic::utility;

using my_type_list = traits::type_container<int, char, bool, int, char, long, double>;
using my_type_set = my_type_list::apply_t<traits::type_set>;

static_assert(std::same_as<my_type_list::indexed_t<1>, char>);
static_assert(my_type_list::type_index<int> == 0);
static_assert(std::same_as<traits::empty_type_list::rest_t, traits::empty_type_list>);
static_assert(my_type_set::container::size == 5);

template<std::size_t I>
void print_type()
{
    static constexpr std::string_view str =
        nameof::nameof_full_type_v<typename my_type_set::container::indexed_t<I>>;

    BOOST_TEST_MESSAGE(str);
}

template<std::size_t... I>
struct print_helper
{
    print_helper() { (print_type<I>(), ...); }
};

BOOST_AUTO_TEST_SUITE(util_test)

BOOST_AUTO_TEST_CASE(type_list_test)
{
    constexpr auto size = my_type_set::container::size;
    BOOST_TEST_MESSAGE(fmt::format("size is {}", size));
    traits::apply_index_sequence_t<print_helper, traits::make_index_sequence_v<size>>{};
}

BOOST_AUTO_TEST_SUITE_END()
