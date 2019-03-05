/*********************************
 ** Tsunagari Tile Engine       **
 ** string.h                    **
 ** Copyright 2019 Paul Merrill **
 *********************************/

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

#ifndef SRC_UTIL_STRING_H_
#define SRC_UTIL_STRING_H_

#include "util/string-view.h"
#include "util/vector.h"

class String;

class NullTerminatedString {
 public:
    explicit NullTerminatedString(String& s);
    ~NullTerminatedString();

    const char* get();
    operator const char*();

 public:
    String& s;
};

class String : public vector<char> {
 public:
    String() = default;
    String(const char* value);
    String(StringView value);
    String(const String& other) = default;
    String(String&& other);
    ~String() = default;

    String& operator=(const String& other) = default;
    String& operator=(String&& other);
    bool operator<(const String& other) const noexcept;
    bool operator>(const String& other) const noexcept;

    String& operator<<(char value);
    String& operator<<(const char* value);
    String& operator<<(StringView value);

    String& operator<<(bool value);
    String& operator<<(int value);
    String& operator<<(unsigned int value);
    String& operator<<(long value);
    String& operator<<(unsigned long value);
    String& operator<<(long long value);
    String& operator<<(unsigned long long value);
    String& operator<<(float value);
    String& operator<<(double value);

    operator StringView() const;
    StringView view() const;

    NullTerminatedString null();
};

#endif  // SRC_UTIL_STRING_H_
