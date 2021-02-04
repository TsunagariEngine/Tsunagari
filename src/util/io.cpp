/*************************************
** Tsunagari Tile Engine            **
** io.cpp                           **
** Copyright 2020-2021 Paul Merrill **
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

#include "util/io.h"

#include "os/c.h"
#include "os/io.h"
#include "util/int.h"
#include "util/noexcept.h"
#include "util/string.h"
#include "util/string-view.h"

struct State {
    State(bool err) : err(err) {}

    bool err;
    String buf;
};

#define STATE (*reinterpret_cast<State*>(data))

static State outstate(false);
static State errstate(true);

Output sout(&outstate);
Output serr(&errstate);

// FIXME: Flush manually instead of automatically on every '\n'.

static void
flush(State& state) noexcept {
    String& buf = state.buf;
    char* data = buf.data;
    size_t size = buf.size;

    if (data[size - 1] == '\n') {
        if (state.err) {
            writeStderr(data, size);
        }
        else {
            writeStdout(data, size);
        }
        buf.clear();
    }
    else {
        size_t len = state.buf.view().rfind('\n') + 1;
        if (state.err) {
            writeStderr(data, len);
        }
        else {
            writeStdout(data, len);
        }
        memmove(data, data + len, size - len);
        buf.size -= len;
    }
}

Output::Output(void* data) noexcept : data(data) {}

Output&
Output::operator<<(char x) noexcept {
    STATE.buf << x;
    if (x == '\n') {
        flush(STATE);
    }
    return *this;
}

Output&
Output::operator<<(const char* x) noexcept {
    *this << StringView(x, strlen(x));
    return *this;
}

Output&
Output::operator<<(StringView x) noexcept {
    STATE.buf << x;
    if (x.find('\n') != SV_NOT_FOUND) {
        flush(STATE);
    }
    return *this;
}

Output&
Output::operator<<(bool x) noexcept {
    if (x) {
        STATE.buf << StringView("true", 4);
    }
    else {
        STATE.buf << StringView("false", 5);
    }
    return *this;
}

Output&
Output::operator<<(int x) noexcept {
    STATE.buf << x;
    return *this;
}

Output&
Output::operator<<(unsigned int x) noexcept {
    STATE.buf << x;
    return *this;
}

Output&
Output::operator<<(long x) noexcept {
    STATE.buf << x;
    return *this;
}

Output&
Output::operator<<(unsigned long x) noexcept {
    STATE.buf << x;
    return *this;
}

Output&
Output::operator<<(long long x) noexcept {
    STATE.buf << x;
    return *this;
}

Output&
Output::operator<<(unsigned long long x) noexcept {
    STATE.buf << x;
    return *this;
}

Output&
Output::operator<<(float x) noexcept {
    STATE.buf << x;
    return *this;
}
