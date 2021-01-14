/*************************************
** Tsunagari Tile Engine            **
** c.h                              **
** Copyright 2019-2021 Paul Merrill **
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

#ifndef SRC_OS_C_H_
#define SRC_OS_C_H_

#if defined(_WIN32)
#include "os/windows-c.h"
#elif defined(__APPLE__)
#include "os/mac-c.h"
#elif defined(__linux__) || defined(__EMSCRIPTEN__)
#include "os/linux-c.h"
#elif defined(__FreeBSD__)
#include "os/freebsd-c.h"
#elif defined(__NetBSD__)
#include "os/netbsd-c.h"
#else
#error Not implemented yet
#endif

#endif  // SRC_OS_C_H_
