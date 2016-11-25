/***************************************
** Tsunagari Tile Engine              **
** animation.h                        **
** Copyright 2011-2013 PariahSoft LLC **
** Copyright 2016 Paul Merrill        **
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


#ifndef ANIMATED_H
#define ANIMATED_H

#include <time.h>

#include <memory>
#include <vector>

class Image;

#define ANIM_INFINITE_CYCLES -1

/**
 * An Animation is a sequence of bitmap images (called frames) used to creates
 * the illusion of motion. Frames are cycled over with an even amount of time
 * given to each, and the whole animation starts over after the last frame is
 * displayed.
 *
 * Mechanically, it is a list of images and a period of time over which to
 * play.
 */
class Animation
{
public:
    /**
     * Constructs an empty, but safe, Animation. All methods on this
     * object will be null.
     */
    Animation();

    /**
     * Constructs a single-frame Animation. It will function like a static
     * image.
     *
     * @param frame static image
     */
    Animation(const std::shared_ptr<Image>& frame);

    /**
     * Constructs a Animation from a list of frames.
     *
     * If given more than one frame, frameTime must be a positive,
     * non-zero value.
     *
     * @param frames list of frames to cycle through
     * @param frameTime length of time in milliseconds that each frame
     *        will display for
     */
    Animation(const std::vector<std::shared_ptr<Image>>& frames,
        time_t frameTime);

    /**
     * Starts the animation over.
     *
     * @now current time in milliseconds
     * @cycles number of animation cycles to play
     *   -1 : infinite
     *    0 : none, static image
     *   >0 : limited number of cycles, will stop on last frame of last cycle
     */
    void startOver(time_t now, int cycles);

    /**
     * Has this Animation switched frames since frame() was last called?
     *
     * @now current time in milliseconds
     */
    bool needsRedraw(time_t now) const;

    /**
     * Returns the image that should be displayed at this time.
     *
     * @now current time in milliseconds
     */
    Image* frame(time_t now);

private:

    typedef std::vector<std::shared_ptr<Image>> ImageVec;


    /** List of images in animation. */
    ImageVec frames;

    /**
     * How many animation cycles are we doing?
     * -1 : infinite
     *  0 : none, static image
     * >0 : limited number of cycles, will stop on last frame of last cycle
     */
    int cycles;

    /** Length of each frame in animation in milliseconds. */
    time_t frameTime;

    /** Length of one complete cycle through animation in milliseconds. */
    time_t cycleTime;

    /** Index of frame currently displaying on screen. */
    size_t frameShowing;

    /** Time offset to find current animation frame. */
    time_t offset;
};

#endif
