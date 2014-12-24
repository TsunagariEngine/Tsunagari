/***************************************
** Tsunagari Tile Engine              **
** overlay.cpp                        **
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

#include "area.h"
#include "client-conf.h"
#include "overlay.h"

Overlay::Overlay()
{
}

Overlay::~Overlay()
{
}

void Overlay::teleport(int x, int y)
{
	r.x = x;
	r.y = y;
}

void Overlay::move(int x, int y)
{
	fromTile = NULL;
	destTile = NULL;

	// XXX: Find nearest facing.
	//setFacing(ivec2(x, y));

	// XXX: Hack for facing. Shouldn't need to set.
	setFacing(ivec2(0, 1));

	// Copied from Entity::preMove()
	destCoord = r + rcoord(x, y, 0);

	moving = true;

	// Start moving animation.
	//setPhase("moving " + getFacing());

	// Movement happens over time. See updateTile().
}

void Overlay::erase()
{
	area->erase(this);
}

