
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_NUM_TRAITS_SCALE
#define CNL_IMPL_NUM_TRAITS_SCALE

#include "../power.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/is_integral.h"
#include "../type_traits/is_signed.h"

namespace cnl {
    template<int Digits, int Radix, class S, class Enable = void>
    struct scale;

    namespace _impl {
        // fundamental integer-friendly cnl::scale algorithm
        template<int Digits, int Radix, typename S, class Enable = void>
        struct default_scale;

        template<int Digits, int Radix, typename S>
        struct default_scale<Digits, Radix, S, _impl::enable_if_t<0<=Digits>> {
            constexpr auto operator()(S const& s) const
            -> decltype(s*cnl::power<S, Digits, Radix>())
            {
                return s*cnl::power<S, Digits, Radix>();
            }
        };

#if 0
        template<int Digits, typename S>
        struct default_scale<Digits, 2, S, _impl::enable_if_t<0<=Digits&&!is_signed<S>::value>> {
            constexpr auto operator()(S const& s) const
            -> decltype(s<<constant<Digits>{})
            {
                return s<<constant<Digits>{};
            }
        };

        template<int Digits, typename S>
        struct default_scale<Digits, 4, S, _impl::enable_if_t<0<=Digits>>
                : default_scale<Digits*2, 2, S> {
        };

        template<int Digits, typename S>
        struct default_scale<Digits, 8, S, _impl::enable_if_t<0<=Digits>>
                : default_scale<Digits*3, 2, S> {
        };

        template<int Digits, typename S>
        struct default_scale<Digits, 16, S, _impl::enable_if_t<0<=Digits>>
                : default_scale<Digits*4, 2, S> {
        };
#endif

        // cnl::default_scale<-ve, cnl::constant<>>
        template<int Digits, int Radix, typename S>
        struct default_scale<Digits, Radix, S, _impl::enable_if_t<Digits<0>> {
            constexpr auto operator()(S const& s) const
            -> decltype(s/cnl::power<S, -Digits, Radix>())
            {
                return s/cnl::power<S, -Digits, Radix>();
            }
        };
    }

    // cnl::scale<..., fundamental-integer>
    template<int Digits, int Radix, class S>
    struct scale<Digits, Radix, S, _impl::enable_if_t<cnl::_impl::is_integral<S>::value>>
            : _impl::default_scale<Digits, Radix, S> {
    };

    namespace _impl {
        // cnl::_impl::scale - convenience wrapper for cnl::scale
        template<int Digits, int Radix=2, class S>
        constexpr auto scale(S const& s)
        -> decltype(cnl::scale<Digits, Radix, S>{}(s))
        {
            return cnl::scale<Digits, Radix, S>{}(s);
        }
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_SCALE
