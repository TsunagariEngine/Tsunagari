/*************************************
** Tsunagari Tile Engine            **
** string-view.cpp                  **
** Copyright 2019-2020 Paul Merrill **
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

#include "util/string-view.h"

#include "os/c.h"
#include "util/fnv.h"
#include "util/noexcept.h"

StringPosition
StringView::find(char needle) const noexcept {
    char* result = static_cast<char*>(memchr(data, needle, size));
    if (result == nullptr) {
        return SV_NOT_FOUND;
    }
    return StringPosition(result - data);
}

StringPosition
StringView::find(StringView needle) const noexcept {
    char* result =
            static_cast<char*>(memmem(data, size, needle.data, needle.size));
    if (result == nullptr) {
        return SV_NOT_FOUND;
    }
    return StringPosition(result - data);
}

StringPosition
StringView::find(StringView needle, size_t start) const noexcept {
    assert_(size >= start);

    char* result = static_cast<char*>(
            memmem(data + start, size - start, needle.data, needle.size));
    if (result == nullptr) {
        return SV_NOT_FOUND;
    }
    return StringPosition(result - data);
}

StringPosition
StringView::rfind(char needle) const noexcept {
    if (size == 0) {
        return SV_NOT_FOUND;
    }

    size_t i = size - 1;
    do {
        if (data[i] == needle) {
            return StringPosition(i);
        }
        i--;
    } while (i > 0);

    return SV_NOT_FOUND;
}

size_t
hash_(StringView s) noexcept {
    return fnvHash(s.data, s.size);
}

size_t
hash_(const char* s) noexcept {
    return hash_(StringView(s));
}
