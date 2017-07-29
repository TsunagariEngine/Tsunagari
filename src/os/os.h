/***************************************
** Tsunagari Tile Engine              **
** os/os.h                            **
** Copyright 2016-2017 Paul Merrill   **
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

#ifndef SRC_OS_OS_H_
#define SRC_OS_OS_H_

#include <stdint.h>

#include <string>
#include <vector>

extern char dirSeparator;

uint64_t getFileSize(const std::string& path);
bool isDir(const std::string& path);
void makeDirectory(const std::string& path);
std::vector<std::string> listDir(const std::string& path);

enum TermColor {
    TC_RESET,
    TC_GREEN,
    TC_YELLOW,
    TC_RED,
};

void setTermColor(TermColor color);

#endif  // SRC_OS_OS_H_
