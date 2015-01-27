/***************************************
** Tsunagari Tile Engine              **
** reader.h                           **
** Copyright 2011-2015 PariahSoft LLC **
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

#ifndef READER_H
#define READER_H

#include <memory>
#include <string>

namespace Gosu {
	class Buffer;
}

/**
 * FIXME
 * Provides data and resource extraction for a World.
 * Each World comes bundled with associated data in the form of a Zip file.
 * A Reader object knows how to navigate the data, extract individual
 * requested files, and process the files into data structures. The final data
 * structures are kept in memory for future requests.
 */
class Reader
{
public:
	static bool init();

	static bool prependPath(const std::string& path);

	//! Returns true if the World contains a resource by that name.
	static bool resourceExists(const std::string& name);
	static bool directoryExists(const std::string& name);
	static bool fileExists(const std::string& name);

	static Gosu::Buffer* readBuffer(const std::string& name);
	static std::string readString(const std::string& name);

	//! Request a text file from the World.
	static std::string getText(const std::string& name);

	//! Expunge old resources cached in memory. Decisions on which are
	//! removed and which are kept are based on the global Conf struct.
	static void garbageCollect();
};

#endif

