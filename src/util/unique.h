/*************************************
** Tsunagari Tile Engine            **
** unique.h                         **
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

#ifndef SRC_UTIL_UNIQUE_H_
#define SRC_UTIL_UNIQUE_H_

#include "util/assert.h"
#include "util/noexcept.h"

//
// Pointers
//   unique.h
//     class Unique      Unique pointer
//   rc.h
//     class Rc          Reference counted pointer
//     class CompactRc   Half the size and half the number of allocations
//

// Unique pointers delete their object when destroyed.
template<typename T>
class Unique {
    T* x;

 public:
    Unique() noexcept : x(nullptr) {}
    Unique(Unique&& other) noexcept : x(other.x) { other.x = nullptr; }
    Unique(T* x) noexcept : x(x) {}
    ~Unique() noexcept { delete x; }

    void
    operator=(T* x) noexcept {
        delete this->x;
        this->x = x;
    }

    void
    operator=(Unique&& other) noexcept {
        delete x;
        x = other.x;
        other.x = nullptr;
    }

    operator bool() const noexcept { return x != nullptr; }

    T*
    get() const noexcept {
        return x;
    }
    T* operator->() const noexcept {
        assert_(x);
        return x;
    }
    T& operator*() const noexcept { return *x; }

 private:
    // Unique pointers cannot be copied.
    Unique(const Unique&) noexcept {}
    void
    operator=(const Unique&) noexcept {}
};

#endif  // SRC_UTIL_UNIQUE_H_
