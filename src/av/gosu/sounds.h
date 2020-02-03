/***************************************
** Tsunagari Tile Engine              **
** sounds.h                           **
** Copyright 2011-2014 Michael Reiley **
** Copyright 2011-2020 Paul Merrill   **
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

#ifndef SRC_AV_GOSU_GOSU_SOUNDS_H_
#define SRC_AV_GOSU_GOSU_SOUNDS_H_

#include "av/gosu/gosu.h"
#include "cache/rc-cache-impl.h"
#include "cache/rc-reader-cache.h"
#include "core/sounds.h"
#include "util/rc.h"

class GosuPlayingSound : public PlayingSound {
 public:
    GosuPlayingSound(Gosu::Channel instance);

    ~GosuPlayingSound() = default;

    bool
    playing();
    void
    stop();

    bool
    paused();
    void
    pause();
    void
    resume();

    void
    volume(double volume);
    void
    pan(double pan);
    void
    speed(double speed);

 private:
    GosuPlayingSound() = delete;
    GosuPlayingSound(const GosuPlayingSound&) = delete;
    GosuPlayingSound&
    operator=(const GosuPlayingSound&) = delete;

    Gosu::Channel instance;
};


Rc<Gosu::Sample>
genSample(const std::string& name);

class GosuSounds : public Sounds {
 public:
    GosuSounds() = default;
    ~GosuSounds() = default;

    Rc<SoundInstance>
    play(const std::string& path);

    void
    garbageCollect();

 private:
    GosuSounds(const GosuSounds&) = delete;
    GosuSounds&
    operator=(const GosuSounds&) = delete;

    ReaderCache<Rc<Gosu::Sample>, genSample> samples;
};

#endif  // SRC_AV_GOSU_GOSU_SOUNDS_H_
