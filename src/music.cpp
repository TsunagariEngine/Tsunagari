/***************************************
** Tsunagari Tile Engine              **
** music.cpp                          **
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

#include <Gosu/Math.hpp>

#include "client-conf.h"
#include "music.h"
#include "reader.h"

static Music::SongRef genSong(const std::string& name)
{
	std::unique_ptr<Gosu::Buffer> buffer(Reader::readBuffer(name));
	if (!buffer)
		return Music::SongRef();
	return Music::SongRef(new Gosu::Song(buffer->frontReader()));
}



Music::SongRef Music::getSong(const std::string& name)
{
	if (!conf.audioEnabled)
		return SongRef();
	return songs.lifetimeRequest(name);
}

static Music globalMusic;

Music& Music::instance()
{
	return globalMusic;
}


/*
static const char* stateStr(MUSIC_STATE state)
{
	switch (state) {
	case NOT_PLAYING:
		return "NOT_PLAYING";
	case PLAYING_INTRO:
		return "PLAYING_INTRO";
	case PLAYING_LOOP:
		return "PLAYING_LOOP";
	case CHANGED_INTRO:
		return "CHANGED_INTRO";
	case CHANGED_LOOP:
		return "CHANGED_LOOP";
	default:
		return "";
	}
}
*/

void Music::setState(MUSIC_STATE state_)
{
	// printf("State changed from %s to %s.\n", stateStr(this->state), stateStr(state));
	state = state_;
}

void Music::playIntro()
{
	if (musicInst && musicInst->playing())
		musicInst->stop();
	curIntro = newIntro;
	introMusic->play(false);
	introMusic->changeVolume(conf.musicVolume / 100.0);
	musicInst = introMusic;
	setState(PLAYING_INTRO);
}

void Music::playLoop()
{
	if (musicInst && musicInst->playing())
		musicInst->stop();
	curLoop = newLoop;
	loopMusic->play(true);
	loopMusic->changeVolume(conf.musicVolume / 100.0);
	musicInst = loopMusic;
	setState(PLAYING_LOOP);
}



Music::Music() : songs(genSong), paused(false), state(NOT_PLAYING)
{
}

Music::~Music()
{
	if (musicInst && musicInst->playing())
		musicInst->stop();
}

std::string Music::getIntro()
{
	return newIntro;
}

std::string Music::getLoop()
{
	return newLoop;
}

void Music::setIntro(const std::string& filename)
{
	if (newIntro == filename)
		return;

	switch (state) {
	case NOT_PLAYING:
	case PLAYING_INTRO:
	case PLAYING_LOOP:
		setState(CHANGED_INTRO);
	default: break;
	}

	newIntro = filename;
	// Optimize XXX: Don't load until played.
	introMusic = filename.size() ? getSong(filename) : SongRef();
}

void Music::setLoop(const std::string& filename)
{
	if (newLoop == filename)
		return;

	switch (state) {
	case NOT_PLAYING:
	case PLAYING_INTRO:
	case PLAYING_LOOP:
		setState(CHANGED_LOOP);
	default: break;
	}

	newLoop = filename;
	// Optimize XXX: Don't load until played.
	loopMusic = filename.size() ? getSong(filename) : SongRef();
}

int Music::getVolume()
{
	return conf.musicVolume;
}

void Music::setVolume(int level)
{
	if (0 < level || level > 100) {
		Log::info("Music", "volume can only be set between 0 and 100");
		level = Gosu::clamp(level, 0, 100);
	}
	conf.musicVolume = level;
	if (musicInst)
		musicInst->changeVolume(level);
}

bool Music::isPaused()
{
	return paused;
}

void Music::setPaused(bool p)
{
	if (paused == p)
		return;
	paused = p;
	if (musicInst) {
		if (p)
			musicInst->pause();
		else
			musicInst->play();
	}
}

void Music::stop()
{
	paused = false;
	if (musicInst)
		musicInst->stop();
	state = NOT_PLAYING;
}

void Music::tick()
{
	if (paused)
		return;

	switch (state) {
	case NOT_PLAYING:
		if (musicInst && musicInst->playing())
			musicInst->stop();
		break;
	case PLAYING_INTRO:
		if (!musicInst->playing()) {
			if (newLoop.size() && loopMusic)
				playLoop();
			else
				setState(NOT_PLAYING);
		}
		break;
	case PLAYING_LOOP:
		break;
	case CHANGED_INTRO:
		if (newIntro.size() && introMusic)
			playIntro();
		else if (newLoop.size() && newLoop != curLoop)
			setState(CHANGED_LOOP);
		else if (newLoop.size())
			setState(PLAYING_LOOP);
		else
			setState(NOT_PLAYING);
		break;
	case CHANGED_LOOP:
		if (newIntro.size() && loopMusic)
			playIntro();
		else if (newLoop.size() && loopMusic)
			playLoop();
		else
			setState(NOT_PLAYING);
		break;
	}
}


void exportMusic()
{
}

