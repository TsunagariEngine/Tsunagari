/***************************************
** Tsunagari Tile Engine              **
** reader.cpp                         **
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

#include <errno.h>
#include <stdlib.h>

#include <Gosu/Bitmap.hpp>
#include <Gosu/Image.hpp>
#include <Gosu/IO.hpp>
#include <map>
#include <physfs.h>

#include "cache.h"
#include "cache-template.cpp"
#include "client-conf.h"
#include "dtds.h"
#include "formatter.h"
#include "log.h"
#include "reader.h"
#include "window.h"
#include "xml.h"

#include "data/world.h"

#define ASSERT(x)  if (!(x)) { return false; }

typedef std::shared_ptr<xmlDtd> DTDRef;
typedef std::shared_ptr<std::string> StringRef;


// Caches that store processed, game-ready objects. Garbage collected.
static Cache<ImageRef> images;
static Cache<TiledImageRef> tiles;
static Cache<XMLRef> xmls;
static Cache<StringRef> texts;

// DTDs don't expire. No garbage collection.
typedef std::map<std::string, DTDRef> DTDMap;
static DTDMap dtds;



static std::string path(const std::string& entryName)
{
	// XXX: archive might not be world
	return DataWorld::instance().datafile + "/" + entryName;
}

template <class T>
static bool readFromDisk(const std::string& name, T& buf)
{
	PHYSFS_sint64 size;
	PHYSFS_File* zf;

	if (!PHYSFS_exists(name.c_str())) {
		Log::err("Reader", Formatter("%: file missing")
				% path(name));
		return false;
	}

	zf = PHYSFS_openRead(name.c_str());
	if (!zf) {
		Log::err("Reader", Formatter("%: error opening file: %")
				% path(name) % PHYSFS_getLastError());
		return false;
	}

	size = PHYSFS_fileLength(zf);
	if (size == -1) {
		Log::err("Reader", Formatter("%: could not determine file size: %")
				% path(name) % PHYSFS_getLastError());
		PHYSFS_close(zf);
		return false;
	}
	else if (size > std::numeric_limits<uint32_t>::max()) {
		// FIXME: Technically, we just need to issue multiple calls to
		// PHYSFS_read. Fix when needed.
		Log::err("Reader", Formatter("%: file too long (>4GB)")
				% path(name));
		PHYSFS_close(zf);
		return false;
	}
	else if (size < -1) {
		Log::err("Reader", Formatter("%: invalid file size: %")
				% path(name) % PHYSFS_getLastError());
		PHYSFS_close(zf);
		return false;
	}

	buf.resize((size_t)size);
	if (size == 0) {
		PHYSFS_close(zf);
		return true;
	}

	if (PHYSFS_read(zf, (char*)(buf.data()),
			(PHYSFS_uint32)size, 1) != 1) {
		Log::err("Reader", Formatter("%: error reading file: %")
				% path(name) % PHYSFS_getLastError());
		PHYSFS_close(zf);
		return false;
	}

	PHYSFS_close(zf);
	return true;
}

// FIXME: Should be moved to xml.cpp!!!!!!
static DTDRef parseDTD(const std::string& dtdContent)
{
	xmlCharEncoding enc = XML_CHAR_ENCODING_NONE;

	xmlParserInputBuffer* input = xmlParserInputBufferCreateMem(
			dtdContent.c_str(), (int)dtdContent.size(), enc);
	if (!input)
		return DTDRef();

	xmlDtd* dtd = xmlIOParseDTD(NULL, input, enc);
	if (!dtd)
		return DTDRef();

	return DTDRef(dtd, xmlFreeDtd);
}

// FIXME: Should be moved to xml.cpp!!!!!!
static bool preloadDTDs()
{
	ASSERT(dtds["area"] = parseDTD(CONTENT_OF_AREA_DTD()));
	ASSERT(dtds["entity"] = parseDTD(CONTENT_OF_ENTITY_DTD()));
	ASSERT(dtds["tsx"] = parseDTD(CONTENT_OF_TSX_DTD()));
	return true;
}

static xmlDtd* getDTD(const std::string& type)
{
	DTDMap::iterator it = dtds.find(type);
	return it == dtds.end() ? NULL : it->second.get();
}

static XMLDoc* readXMLDoc(const std::string& name,
                          const std::string& dtdType)
{
	std::string p = path(name);
	std::string data = Reader::readString(name);
	xmlDtd* dtd = getDTD(dtdType);

	if (!dtd || data.empty())
		return NULL;
	XMLDoc* doc = new XMLDoc;
	if (!doc->init(p, data, dtd)) {
		delete doc;
		return NULL;
	}
	return doc;
}








bool Reader::init()
{
	ASSERT(prependPath(BASE_ZIP_PATH));

	// DTDs must be loaded from BASE_ZIP. They cannot be allowed to be
	// loaded from the world.
	ASSERT(preloadDTDs());

	ASSERT(prependPath(DataWorld::instance().datafile));

	return true;
}

bool Reader::prependPath(const std::string& path)
{
	int err = PHYSFS_mount(path.c_str(), NULL, 0);
	if (err == 0) {
		Log::fatal("Reader", Formatter("%: could not open archive: %")
				% path % PHYSFS_getLastError());
		return false;
	}

	return true;
}

bool Reader::resourceExists(const std::string& name)
{
	return PHYSFS_exists(name.c_str());
}

bool Reader::directoryExists(const std::string& name)
{
	return resourceExists(name) && PHYSFS_isDirectory(name.c_str());
}

bool Reader::fileExists(const std::string& name)
{
	return resourceExists(name) && !PHYSFS_isDirectory(name.c_str());
}

Gosu::Buffer* Reader::readBuffer(const std::string& name)
{
	Gosu::Buffer* buf = new Gosu::Buffer();

	if (readFromDisk(name, *buf)) {
		return buf;
	}
	else {
		delete buf;
		return NULL;
	}
}

std::string Reader::readString(const std::string& name)
{
	std::string str;
	return readFromDisk(name, str) ? str : "";
}

ImageRef Reader::getImage(const std::string& name)
{
	ImageRef existing = images.lifetimeRequest(name);
	if (existing)
		return existing;

	std::unique_ptr<Gosu::Buffer> buffer(readBuffer(name));
	if (!buffer)
		return ImageRef();

	ImageRef result(Image::create(buffer->data(), buffer->size()));
	if (!result)
		return ImageRef();

	images.lifetimePut(name, result);
	return result;
}

TiledImageRef Reader::getTiledImage(const std::string& name,
		int w, int h)
{
	TiledImageRef existing = tiles.momentaryRequest(name);
	if (existing)
		return existing;

	if (w <= 0 || h <= 0)
		return TiledImageRef();

	std::unique_ptr<Gosu::Buffer> buffer(readBuffer(name));
	if (!buffer)
		return TiledImageRef();

	TiledImageRef result(
		TiledImage::create(buffer->data(), buffer->size(),
			(unsigned int)w, (unsigned int)h)
	);
	if (!result)
		return TiledImageRef();

	tiles.momentaryPut(name, result);
	return result;
}

XMLRef Reader::getXMLDoc(const std::string& name,
                            const std::string& dtdType)
{
	XMLRef existing = xmls.momentaryRequest(name);
	if (existing)
		return existing;

	XMLRef result(readXMLDoc(name, dtdType));

	xmls.momentaryPut(name, result);
	return result;
}

std::string Reader::getText(const std::string& name)
{
	StringRef existing = texts.momentaryRequest(name);
	if (existing)
		return *existing.get();

	StringRef result(new std::string(readString(name)));

	texts.momentaryPut(name, result);
	return *result.get();
}

void Reader::garbageCollect()
{
	images.garbageCollect();
	tiles.garbageCollect();
	Sounds::instance().garbageCollect();
	// songs.garbageCollect();
	xmls.garbageCollect();
	texts.garbageCollect();
}

