/***************************************
** Tsunagari Tile Engine              **
** cbuffer.h                          **
** Copyright 2011-2015 Michael Reiley **
** Copyright 2011-2019 Paul Merrill   **
***************************************/

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

#ifndef SRC_AV_GOSU_CBUFFER_H_
#define SRC_AV_GOSU_CBUFFER_H_

#include "av/gosu/gosu.h"

// Similar to Gosu::Buffer, but can be constructed around pre-existing C void*.
// The memory is not copied. Also, this is a read-only implementation. If you
// attempt to create a Gosu::Writer around this, it will fail.
//
// See Gosu/IO.hpp and GosuImpl/IO.cpp from Gosu
// See http://www.libgosu.org/cpp/class_gosu_1_1_buffer.html
// See http://www.libgosu.org/cpp/class_gosu_1_1_resource.html
class GosuCBuffer : public Gosu::Resource {
 public:
    const char* data_;
    size_t size_;

    size_t size() noexcept;
    void resize(size_t) noexcept;  // NOOP
    void read(size_t offset, size_t length, void* destBuffer) noexcept;
    void write(size_t, size_t, const void*) noexcept;  // NOOP
};

#endif  // SRC_AV_GOSU_CBUFFER_H_
