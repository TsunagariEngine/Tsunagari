/*************************************
** Tsunagari Tile Engine            **
** move.h                           **
** Copyright 2017-2020 Paul Merrill **
*************************************/

// **********
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// **********

#ifndef SRC_UTIL_MOVE_H_
#define SRC_UTIL_MOVE_H_

#include "util/constexpr.h"
#include "util/noexcept.h"

template<typename T>
struct Refless_ {
    typedef T value;
};
template<typename T>
struct Refless_<T&&> {
    typedef T value;
};
template<typename T>
struct Refless_<T&> {
    typedef T value;
};
template<typename T>
using Refless = typename Refless_<T>::value;

//
// Move
//   move_()  same as std::move
//

template<typename T>
inline CONSTEXPR11 Refless<T>&&
move_(T&& x) noexcept {
    return static_cast<Refless<T>&&>(x);
}

//
// Forward
//   forward_()  same as std::forward
//

template<typename T>
inline CONSTEXPR11 T&&
forward_(Refless<T>& x) noexcept {
    return static_cast<T&&>(x);
}

template<typename T>
inline CONSTEXPR11 T&&
forward_(Refless<T>&& x) noexcept {
    return static_cast<T&&>(x);
}

//
// Swap
//   swap_()  same as std::swap
//

template<typename T>
inline void
swap_(T& a, T& b) noexcept {
    T temp(move_(a));
    a = move_(b);
    b = move_(temp);
}

#endif  // SRC_UTIL_MOVE_H_
