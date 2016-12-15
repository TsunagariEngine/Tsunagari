/***************************************
** Tsunagari Tile Engine              **
** viewport.h                         **
** Copyright 2011-2013 Michael Reiley **
** Copyright 2011-2016 Paul Merrill   **
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

#ifndef SRC_CORE_VIEWPORT_H_
#define SRC_CORE_VIEWPORT_H_

class Area;
class Entity;

//! General control over where and how the map is rendered.
/*!

 */
class Viewport {
 public:
    static Viewport& instance();

    Viewport();
    ~Viewport() = default;

    void setSize(rvec2 virtRes);

    void tick(time_t dt);
    void turn();

    //! How far the map is scrolled in pixels, counting from the upper-left.
    rvec2 getMapOffset() const;

    //! Size of the letterbox matte bars in pixels.
    rvec2 getLetterboxOffset() const;

    //! Multiplier in X and Y dimensions to get from virtRes to physRes.
    rvec2 getScale() const;

    //! The resolution our game is actually being drawn at.
    rvec2 getPhysRes() const;

    //! The resolution our game thinks it is being drawn at. Chosen by a
    //! world's creator. This allows graphics to look the same on any
    //! setups of any resolution.
    rvec2 getVirtRes() const;

    // Immediatly center render offset. Stop any tracking.
    void jumpToPt(ivec2 pt);
    void jumpToPt(rvec2 pt);
    void jumpToEntity(const Entity* e);

    // Continuously follow.
    void trackEntity(const Entity* e);

    void setArea(const Area* a);

 private:
    void update();

    void _jumpToEntity(const Entity* e);

    //! Returns as a normalized vector the percentage of screen that should
    //! be blanked to preserve the aspect ratio. It can also be thought of
    //! as the correcting aspect ratio.
    rvec2 getLetterbox() const;

    rvec2 offsetForPt(rvec2 pt) const;
    rvec2 centerOn(rvec2 pt) const;
    rvec2 boundToArea(rvec2 pt) const;
    double boundDimension(double window, double area, double pt,
                          bool loop) const;
    rvec2 addLetterboxOffset(rvec2 pt) const;

    enum TrackingMode {
        TM_MANUAL,
        TM_FOLLOW_ENTITY
    };

    double aspectRatio;
    rvec2 off;
    rvec2 virtRes;

    TrackingMode mode;
    const Area* area;
    const Entity* targete;
};

#endif  // SRC_CORE_VIEWPORT_H_
