/***************************************
** Tsunagari Tile Engine              **
** windows.cpp                        **
** Copyright 2007 Julian Raschke      **
** Copyright 2011-2013 Michael Reiley **
** Copyright 2011-2019 Paul Merrill   **
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

#include "os/windows.h"

//#include "core/world.h"

#include "os/c.h"
#include "os/os.h"
#include "util/move.h"
#include "util/noexcept.h"
#include "util/optional.h"
#include "util/string-view.h"
#include "util/string.h"
#include "util/vector.h"

#pragma comment(linker, \
                "\"/manifestdependency:type='win32' " \
                "name='Microsoft.Windows.Common-Controls' version='6.0.0.0' " \
                "processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

extern "C" {
#define MAX_PATH 260

typedef struct {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME;

typedef struct {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR cFileName[MAX_PATH];
    CHAR cAlternateFileName[14];
} WIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;

WINBASEAPI BOOL WINAPI AttachConsole(DWORD);
WINBASEAPI BOOL WINAPI CloseHandle(HANDLE) noexcept;
WINBASEAPI BOOL WINAPI CreateDirectoryA(LPCSTR, void*) noexcept;
WINBASEAPI HANDLE WINAPI
CreateFileA(LPCSTR, DWORD, DWORD, void*, DWORD, DWORD, HANDLE) noexcept;
WINBASEAPI VOID WINAPI ExitProcess(UINT) noexcept;
WINBASEAPI HANDLE WINAPI FindFirstFileA(LPCSTR, LPWIN32_FIND_DATAA) noexcept;
WINBASEAPI BOOL WINAPI FindNextFileA(HANDLE, LPWIN32_FIND_DATAA) noexcept;
WINBASEAPI BOOL WINAPI FindClose(HANDLE) noexcept;
_ACRTIMP FILE* __cdecl freopen(const char*, const char*, FILE*) noexcept;
WINBASEAPI DWORD WINAPI GetFileAttributesA(LPCSTR) noexcept;
WINBASEAPI BOOL WINAPI GetFileSizeEx(HANDLE, LARGE_INTEGER*) noexcept;
WINBASEAPI HANDLE WINAPI GetStdHandle(DWORD) noexcept;
WINUSERAPI int WINAPI MessageBoxA(HWND, LPCSTR, LPCSTR, UINT) noexcept;
WINBASEAPI BOOL WINAPI ReadFile(HANDLE, VOID*, DWORD, DWORD*, void*) noexcept;
WINBASEAPI BOOL WINAPI SetConsoleTextAttribute(HANDLE, WORD) noexcept;
WINBASEAPI BOOL WINAPI
WriteFile(HANDLE, LPCVOID, DWORD, LPDWORD, void*) noexcept;

typedef WIN32_FIND_DATAA WIN32_FIND_DATA;

#define CREATE_ALWAYS 2
#define CreateDirectory CreateDirectoryA
#define CreateFile CreateFileA
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define FILE_READ_ATTRIBUTES 0x0080
#define FILE_READ_DATA 0x0001
#define FILE_WRITE_DATA (0x0002)
#define FindFirstFile FindFirstFileA
#define FindNextFile FindNextFileA
#define FOREGROUND_BLUE 0x0001
#define FOREGROUND_GREEN 0x0002
#define FOREGROUND_INTENSITY 0x0008
#define FOREGROUND_RED 0x0004
#define GetFileAttributes GetFileAttributesA
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#define MessageBox MessageBoxA
#define OPEN_EXISTING 3
#define STD_OUTPUT_HANDLE ((DWORD)-11)

constexpr DWORD ATTACH_PARENT_PROCESS = -1;
constexpr UINT MB_OK = 0x00000000L;
}

const char dirSeparator = '\\';

Filesize
getFileSize(StringView path) noexcept {
    HANDLE file = CreateFile(String(path).null(),
                             FILE_READ_ATTRIBUTES,
                             0,
                             nullptr,
                             OPEN_EXISTING,
                             0,
                             nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        return mark;
    }

    LARGE_INTEGER size;
    BOOL ok = GetFileSizeEx(file, &size);
    if (!ok) {
        return mark;
    }

    ok = CloseHandle(file);
    (void)ok;
    assert_(ok);

    return Filesize(static_cast<uint64_t>(size.QuadPart));
}

bool
writeFile(StringView path, uint32_t length, void* data) noexcept {
    HANDLE file = CreateFile(String(path).null(),
                             FILE_WRITE_DATA,
                             0,
                             nullptr,
                             CREATE_ALWAYS,
                             0,
                             nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD written;
    BOOL ok = WriteFile(file, data, length, &written, nullptr);
    if (!ok) {
        CloseHandle(file);
        return false;
    }
    if (length != written) {
        CloseHandle(file);
        return false;
    }

    CloseHandle(file);

    return true;
}

bool
writeFileVec(StringView path,
             uint32_t count,
             uint32_t* lengths,
             void** datas) noexcept {
    HANDLE file = CreateFile(String(path).null(),
                             FILE_WRITE_DATA,
                             0,
                             nullptr,
                             CREATE_ALWAYS,
                             0,
                             nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        return false;
    }

    for (uint32_t i = 0; i < count; i++) {
        DWORD written;
        BOOL ok = WriteFile(file, datas[i], lengths[i], &written, nullptr);
        if (!ok) {
            CloseHandle(file);
            return false;
        }
        if (lengths[i] != written) {
            CloseHandle(file);
            return false;
        }
    }

    CloseHandle(file);

    return true;
}

bool
isDir(StringView path) noexcept {
    DWORD attributes = GetFileAttributes(String(path).null());

    return (attributes != INVALID_FILE_ATTRIBUTES &&
            (attributes & FILE_ATTRIBUTE_DIRECTORY));
}

void
makeDirectory(StringView path) noexcept {
    BOOL ok = CreateDirectory(String(path).null(), nullptr);
    (void)ok;
}

Vector<String>
listDir(StringView path) noexcept {
    Vector<String> files;

    String query = path;
    query << "\\*";

    WIN32_FIND_DATA data;
    HANDLE find = FindFirstFile(query.null(), &data);

    if (find == INVALID_HANDLE_VALUE) {
        return Vector<String>();
    }

    do {
        if (data.cFileName[0] == '.' && data.cFileName[1] == '\0' ||
            data.cFileName[1] == '.') {
            continue;
        }

        files.push_back(String(data.cFileName));
    } while (FindNextFile(find, &data));

    FindClose(find);

    return files;
}

Optional<String>
readFile(StringView path) noexcept {
    Filesize size__ = getFileSize(path);
    if (!size__) {
        return none;
    }

	uint64_t size_ = *size__;

	if (size_ > UINT32_MAX) {
        return none;
    }

	DWORD size = static_cast<DWORD>(size_);

    HANDLE file = CreateFile(String(path).null(),
                             FILE_READ_DATA,
                             0,
                             nullptr,
                             OPEN_EXISTING,
                             0,
                             nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        return none;
    }

	String data;
    data.resize(size);

	DWORD read;
    BOOL ok = ReadFile(file, data.data(), size, &read, nullptr);
    if (!ok) {
        return none;
    }
    if (read != size) {
        return none;
    }

    return Optional<String>(move_(data));
}

void
setTermColor(TermColor color, PrintDest /*dest*/) noexcept {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == INVALID_HANDLE_VALUE) {
        return;
    }

    WORD attribute;

	switch (color) {
    default:
    case TC_RESET:
        attribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case TC_GREEN:
        attribute = FOREGROUND_GREEN;
        break;
    case TC_YELLOW:
        attribute = FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    case TC_RED:
        attribute = FOREGROUND_RED;
        break;
    }

    BOOL ok = SetConsoleTextAttribute(out, attribute);
    (void)ok;
}

void
wFixConsole() noexcept {
    //if (AttachConsole(ATTACH_PARENT_PROCESS)) {
    //    freopen("CONOUT$", "wb", stdout);  // Attach STDOUT.
    //    freopen("CONOUT$", "wb", stderr);  // Attach STDERR.
    //}
}

/* From gosu/src/Utility.cpp by Julian Raschke in 2007 */
/*
std::wstring widen(StringView s) noexcept {
  size_t wideLen = mbstowcs(0, String(s).null(), 0);
  if (wideLen == static_cast<size_t>(-1)) {
    throw std::runtime_error("Could not convert from string to wstring: " + s);
  }

  vector<wchar_t> buf(wideLen + 1);
  mbstowcs(&buf.front(), String(s).null(), buf.size());

  return std::wstring(buf.begin(), buf.end() - 1);
}
*/

void
wMessageBox(StringView title, StringView text) noexcept {
    // World::instance().setPaused(true);
    // FIXME: Try to get the window's native handle instead of passing nullptr.
    MessageBox(nullptr, String(text).null(), String(title).null(), MB_OK);
    // World::instance().setPaused(false);
}

void
exitProcess(int code) noexcept {
    ExitProcess(code);
}
