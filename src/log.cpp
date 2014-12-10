/***************************************
** Tsunagari Tile Engine              **
** log.cpp                            **
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

#include <iostream>
#include <sstream>

#include "client-conf.h"
#include "log.h"
#include "window.h"
#include "world.h"

#ifdef _WIN32
	#include "os-windows.h"
#endif

#ifdef __APPLE__
	#include "os-mac.h"
#endif

static verbosity_t verb = V_NORMAL;

static unsigned long startTime;

static std::string& chomp(std::string& str)
{
	std::string::size_type notwhite = str.find_last_not_of(" \t\n\r");
	str.erase(notwhite + 1);
	return str;
}

static std::string ts()
{
	time_t now = GameWindow::time();

	std::ostringstream ts;
	ts.precision(3);
	ts << std::fixed;
	ts << (now - startTime) / (long double)1000.0;
	return "[" + ts.str() + "] ";
}

bool Log::init()
{
	startTime = GameWindow::time();
	return true;
}

void Log::setVerbosity(verbosity_t v)
{
	verb = v;
}

void Log::info(std::string domain, std::string msg)
{
	std::string str = ts() + "Info [" + domain + "] - " + chomp(msg);
	if (verb > V_NORMAL)
		std::cout << str << std::endl;
}

void Log::err(std::string domain, std::string msg)
{
	if (conf.halting == HALT_ERROR) {
		Log::fatal(domain, msg);
		exit(1);
	}
	std::string str = ts() + "Error [" + domain + "] - " + chomp(msg);
	if (verb > V_QUIET) {
		std::cerr << str << std::endl;
		#ifdef _WIN32
			wMessageBox("Tsunagari - Error", str);
		#endif
		#ifdef __APPLE__
			macMessageBox("Tsunagari - Error", str.c_str());
		#endif
	}
}

void Log::fatal(std::string domain, std::string msg)
{
	std::string str = ts() + "Fatal [" + domain + "] - " + chomp(msg);
	std::cerr << str << std::endl;
	#ifdef _WIN32
		wMessageBox("Tsunagari - Fatal", str);
	#endif
	#ifdef __APPLE__
		macMessageBox("Tsunagari - Fatal", str.c_str());
	#endif
}

void Log::reportVerbosityOnStartup()
{
	std::string verbString;
	switch (conf.verbosity)
	{
		case V_QUIET:
			verbString = "QUIET";
			break;
		case V_NORMAL:
			verbString = "NORMAL";
			break;
		case V_VERBOSE:
			verbString = "VERBOSE";
			break;
	}
	std::cout << ts() << "Reporting engine messages in " << verbString
			<< " mode." << std::endl;
}

void exportLog()
{
}

