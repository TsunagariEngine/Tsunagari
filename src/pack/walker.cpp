/*************************************
** Tsunagari Tile Engine            **
** walker.cpp                       **
** Copyright 2017-2019 Paul Merrill **
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

#include "pack/walker.h"

#include "os/os.h"
#include "util/jobs.h"
#include "util/noexcept.h"
#include "util/string-view.h"

struct WalkContext {
    Function<void(StringView)> op;
};

static void
walkPath(WalkContext& ctx, StringView path) noexcept {
    // TODO: The call to isDir can be removed on Unix-like systems, which will
    //       save a stat(), as readdir() returns a file's type already.
    if (isDir(path)) {
        Vector<String> names = listDir(path);
        for (auto& name : names) {
            String child;
            child << path << dirSeparator << name;
            JobsEnqueue([&ctx, child /* = move_(child) */]() noexcept {
                walkPath(ctx, child);
            });
        }
    }
    else {
        ctx.op(path);
    }
}

void
walk(Vector<StringView> paths, Function<void(StringView)> op) noexcept {
    WalkContext ctx;
    ctx.op = move_(op);

    for (auto& path : paths) {
        JobsEnqueue([&]() noexcept { walkPath(ctx, path); });
    }

    JobsFlush();
}
