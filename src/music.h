/***************************************
** Tsunagari Tile Engine              **
** music.h                            **
** Copyright 2011-2014 PariahSoft LLC **
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

#ifndef MUSIC_H
#define MUSIC_H

#include <memory>
#include <string>

/**
 * State manager for currently playing music. Continuously controls which music
 * will play. At the moment, each Area has an INTRO music and a LOOP music.
 * Immediately upon entering an Area, the currently playing music is stopped
 * and the associated intro music started. Once the intro music finishes, or,
 * if there is no intro music, then immediately upon entering said Area, the
 * loop music begins. Loop music is looped forever until either the world is
 * exited or new music is specified, either by a script or by entering a new
 * Area.
 *
 * When switching to a new Area with the same intro or loop music as the
 * previous Area, the music is left alone, if possible.
 */
class Music
{
public:
	static Music& instance();

	virtual ~Music();

	virtual bool setIntro(const std::string& filename);
	virtual bool setLoop(const std::string& filename);

	int getVolume();
	virtual bool setVolume(int level);

	bool isPaused();
	virtual bool setPaused(bool p);

	virtual void stop();

	virtual void tick();

protected:
	Music();

	void playIntro();
	void playLoop();

	enum MUSIC_STATE
	{
		NOT_PLAYING,
		PLAYING_INTRO,
		PLAYING_LOOP,
		CHANGED_INTRO,
		CHANGED_LOOP
	} state;

	int volume;
	bool paused;

	std::string curIntro, newIntro;
	std::string curLoop, newLoop;
};

#endif

