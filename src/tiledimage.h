/***************************************
** Tsunagari Tile Engine              **
** tiledimage.h                       **
** Copyright 2011-2013 PariahSoft LLC **
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

#ifndef TILEDIMAGE_H
#define TILEDIMAGE_H

#include <memory>

class Image;

class TiledImage
{
public:
	static TiledImage* create(void* data, size_t length,
			unsigned tileW, unsigned tileH);
	virtual ~TiledImage();

	virtual size_t size() const = 0;

	virtual std::shared_ptr<Image>& operator[](size_t n) = 0;
	virtual const std::shared_ptr<Image>& operator[](size_t n) const = 0;

private:
	TiledImage();

	friend class TiledImageImpl;
};

#endif

