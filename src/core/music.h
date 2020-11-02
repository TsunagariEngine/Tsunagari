/***************************************
** Tsunagari Tile Engine              **
** music.h                            **
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

#ifndef SRC_CORE_MUSIC_H_
#define SRC_CORE_MUSIC_H_

#include "util/string-view.h"

/**
 * State manager for currently playing music. Continuously controls which music
 * will play. Immediately upon entering an Area, the currently playing music is
 * stopped and the new music started and will play in a loop.
 *
 * When switching to a new Area with the same music as the previous Area, the
 * music is left alone, if possible.
 *
 * When a new music is played, the pause state of the previous music is
 * dropped.
 */

//! If the music path has changed, start playing it.
void
musicPlay(StringView path) noexcept;

//! Stop playing music. To begin again, set a new intro or loop.
void
musicStop() noexcept;

//! Pause playback of music.
void
musicPause() noexcept;
//! Resume playback of music.
void
musicResume() noexcept;

//! Free music not recently played.
void
musicGarbageCollect() noexcept;

#endif  // SRC_CORE_MUSIC_H_
