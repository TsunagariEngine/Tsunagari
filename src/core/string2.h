/***************************************
** Tsunagari Tile Engine              **
** string2.h                          **
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

#ifndef STRING2_H
#define STRING2_H

#include <string>
#include <vector>

//! Returns true if the string contains only digits, whitespace, and minus.
bool isInteger(const std::string& s);

//! Returns true if the string contains only digits, whitespace, minus, and
//! period.
bool isDecimal(const std::string& s);

bool isRanges(const std::string& s);


//! Whether two strings are case-insensative equals.
bool iequals(const std::string& a, const std::string& b);

//! Return a bool from a "true"/"false" string.
bool parseBool(const std::string& s);

int parseUInt(const std::string& s);
int parseInt100(const std::string& s);

//! Split a string by a delimiter.
std::vector<std::string> splitStr(const std::string& str,
    const std::string& delimiter);

/**
 * Parse ranges of integers separated by commas.
 * Can take things such as "5-7,2,12-18".
 */
std::vector<int> parseRanges(const std::string& format);

//! Convert an integer to a representative string.
std::string itostr(int in);

#endif  // STRING2_H
