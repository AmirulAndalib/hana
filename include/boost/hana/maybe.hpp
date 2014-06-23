/*!
@file
Defines `boost::hana::Maybe`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_HPP
#define BOOST_HANA_MAYBE_HPP

#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana {
    /*!
    @ingroup datatypes
    Represents an optional value.

    A `Maybe` either contains a value (represented as `just(x)`), or it is
    empty (represented as `nothing`).

    --------------------------------------------------------------------------

    ## Instance of

    ### Comparable
    Two `Maybe`s are equal if and only if they are both empty or they both
    contain a value and those values are equal.
    @snippet example/maybe/comparable.cpp main

    ### Functor
    A `Maybe` can be seen as a `List` containing either one element (`just(x)`)
    or no elements at all (`nothing`). As such, `fmap` for `Maybe`s returns
    `nothing` when applied to `nothing` and `just(f(x))` when applied to
    `just(x)`.
    @snippet example/maybe/functor.cpp main

    ### Applicative
    First, a value can be made optional with `lift<Maybe>`, which is
    equivalent to `just`. Second, one can feed an optional value to an
    optional function with `ap`, which will return `just(f(x))` if there
    is a function _and_ a value and `nothing` otherwise.
    @include example/maybe/applicative.cpp

    ### Monad
    The `Maybe` `Monad` makes it easy to compose actions that might fail.
    One can feed an optional value if there is one into a function with
    `bind`, which will return `nothing` if there is no value. Finally,
    optional-optional values can have their redundant level of
    `Maybe`ness removed with `join`.
    @snippet example/maybe/monad.cpp main

    @todo
    Check `Applicative` laws.
     */
    struct Maybe { };

    namespace operators {
        template <bool is_valid, typename T>
        struct _maybe {
            using hana_datatype = Maybe;

            template <typename Default, typename F>
            constexpr auto maybe(Default d, F) const
            { return d; }
        };

        template <typename T>
        struct _maybe<true, T> {
            using hana_datatype = Maybe;
            T val;

            template <typename Default, typename F>
            constexpr auto maybe(Default, F f) const
            { return f(val); }
        };

        template <typename T>
        using _just = _maybe<true, T>;
        using _nothing = _maybe<false, void>;
    }

    //! Applies a function to the contents of a `Maybe`, with a fallback
    //! result.
    //! @relates Maybe
    //!
    //! Specifically, `maybe` takes a default value, a function and an
    //! optional value. If the optional value is `nothing`, the default
    //! value is returned. Otherwise, the function is applied to the
    //! content of the `just`.
    //!
    //! ### Example
    //! @snippet example/maybe/api.cpp maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto maybe = [](auto default_, auto f, auto m) {
        return m.maybe(default_, f);
    };

    //! Creates an optional value containing `x`.
    //! @relates Maybe
    //! @hideinitializer
    BOOST_HANA_CONSTEXPR_LAMBDA auto just = [](auto x) {
        return operators::_just<decltype(x)>{x};
    };

    //! Returns whether a `Maybe` contains a value.
    //! @relates Maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto is_just = [](auto m) {
        return maybe(false_, [](auto) { return true_; }, m);
    };

    //! Creates an empty optional value.
    //! @relates Maybe
    //! @hideinitializer
    constexpr operators::_nothing nothing{};

    //! Returns whether a `Maybe` is empty.
    //! @relates Maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto is_nothing = [](auto m) {
        return maybe(true_, [](auto) { return false_; }, m);
    };

    //! Returns the contents of a `Maybe`, or a default value if the `Maybe`
    //! is `nothing`.
    //! @relates Maybe
    //!
    //! ### Example
    //! @snippet example/maybe/api.cpp from_maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto from_maybe = [](auto default_, auto m) {
        return maybe(default_, [](auto x) { return x; }, m);
    };

    //! Extract the value out of a `just`, or trigger a compile-time error
    //! if the argument is `nothing`.
    //! @relates Maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto from_just = [](auto m) {
        auto err = [](auto ...dum) {
            constexpr bool always_false = sizeof...(dum) != 0;
            static_assert(always_false,
            "trying to extract the value inside a boost::hana::nothing "
            "with boost::hana::from_just");
        };
        return maybe(err, [](auto x) { return [=] { return x; }; }, m)();
    };

    template <>
    struct Comparable::instance<Maybe, Maybe> : Comparable::equal_mcd {
        template <typename T, typename U>
        static constexpr auto
        equal_impl(operators::_just<T> t, operators::_just<U> u)
        { return equal(t.val, u.val); }

        template <bool tv, typename T, bool uv, typename U>
        static constexpr auto
        equal_impl(operators::_maybe<tv, T>, operators::_maybe<uv, U>)
        { return bool_<tv == uv>; }
    };

    template <>
    struct Functor::instance<Maybe> : Functor::fmap_mcd {
        template <typename F, typename T>
        static constexpr auto fmap_impl(F f, operators::_just<T> j)
        { return just(f(j.val)); }

        template <typename F>
        static constexpr auto fmap_impl(F, operators::_nothing)
        { return nothing; }
    };

    template <>
    struct Applicative::instance<Maybe> : Applicative::mcd {
        template <typename T>
        static constexpr auto lift_impl(T x)
        { return just(x); }

        template <typename Mf, typename Mx>
        static constexpr auto ap_impl(Mf mf, Mx mx)
        { return maybe(nothing, [=](auto f) { return fmap(f, mx); }, mf); }
    };

    template <>
    struct Monad::instance<Maybe> : Monad::join_mcd {
        template <typename T>
        static constexpr auto join_impl(operators::_just<operators::_just<T>> j)
        { return j.val; }

        static constexpr auto join_impl(operators::_nothing)
        { return nothing; }

        static constexpr auto join_impl(operators::_just<operators::_nothing>)
        { return nothing; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_HPP
