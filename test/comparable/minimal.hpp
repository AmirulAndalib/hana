/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_COMPARABLE_MINIMAL_HPP
#define BOOST_HANA_TEST_COMPARABLE_MINIMAL_HPP

struct MinimalComparable;

template <int i>
struct _comparable { using hana_datatype = MinimalComparable; };

template <int i>
constexpr _comparable<i> comparable{};

#endif // !BOOST_HANA_TEST_COMPARABLE_MINIMAL_HPP
