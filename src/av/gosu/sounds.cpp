/***************************************
** Tsunagari Tile Engine              **
** sounds.cpp                         **
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

#include "av/gosu/sounds.h"

#include "av/gosu/cbuffer.h"
#include "core/client-conf.h"
#include "core/measure.h"
#include "core/resources.h"
#include "core/sounds.h"
#include "util/assert.h"
#include "util/math2.h"
#include "util/string.h"
#include "util/unique.h"

static GosuSounds globalSounds;

Sounds&
Sounds::instance() {
    return globalSounds;
}

GosuSoundInstance::GosuSoundInstance(Gosu::Channel instance)
        : instance(instance) {
    volume(1.0);
}

bool
GosuSoundInstance::playing() {
    return instance.playing();
}

void
GosuSoundInstance::stop() {
    instance.stop();
}

bool
GosuSoundInstance::paused() {
    return instance.paused();
}

void
GosuSoundInstance::pause() {
    instance.pause();
}

void
GosuSoundInstance::resume() {
    instance.resume();
}

void
GosuSoundInstance::volume(double attemptedVolume) {
    double volume = bound(attemptedVolume, 0.0, 1.0);
    if (attemptedVolume != volume) {
        Log::info("SoundInstance",
                  Formatter("Attempted to set volume to %, setting it to %") %
                          attemptedVolume % volume);
    }
    assert_(0 <= conf.soundVolume && conf.soundVolume <= 100);
    instance.set_volume(volume * conf.soundVolume / 100.0);
}

void
GosuSoundInstance::pan(double attemptedPan) {
    double pan = bound(attemptedPan, 0.0, 1.0);
    if (attemptedPan != pan) {
        Log::info("SoundInstance",
                  Formatter("Attempted to set pan to %, setting it to %") %
                          attemptedPan % pan);
    }
    instance.set_pan(pan);
}

void
GosuSoundInstance::speed(double attemptedSpeed) {
    double speed = bound(attemptedSpeed, 0.0, 100.0);
    if (attemptedSpeed != speed) {
        Log::info("SoundInstance",
                  Formatter("Attempted to set speed to %, setting it to %") %
                          attemptedSpeed % speed);
    }
    instance.set_speed(speed);
}


Rc<Gosu::Sample>
genSample(const std::string& path) {
    Unique<Resource> r = Resources::instance().load(path);
    if (!r) {
        // Error logged.
        return Rc<Gosu::Sample>();
    }
    GosuCBuffer buffer(r->data(), r->size());

    TimeMeasure m("Constructed " + path + " as sample");
    return Rc<Gosu::Sample>(new Gosu::Sample(buffer.front_reader()));
}

Rc<SoundInstance>
GosuSounds::play(const std::string& path) {
    auto sample = samples.lifetimeRequest(path);
    if (!sample) {
        // Error logged.
        return Rc<SoundInstance>();
    }
    return Rc<SoundInstance>(new GosuSoundInstance(sample->play()));
}

void
GosuSounds::garbageCollect() {
    samples.garbageCollect();
}
