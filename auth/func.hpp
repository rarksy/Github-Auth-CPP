#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <algorithm>
#include <Wininet.h>
#include <array>
#include <iomanip>
#include <sstream>
#include <lmcons.h>
#include <intrin.h>
#pragma comment(lib, "wininet.lib")

namespace func
{
	void login();
	void Register();
	void Access();

	template<class T>
	inline void log(T content)
	{
		std::cout << content << "\n";
	}
	inline void spacer()
	{
		std::cout << "\n";
	}
	template<class T>
	inline void set_col(T col) {
		std::cout << col;
	}
	inline std::string input(std::string label, std::string& var, bool pword = false)
	{
		if (!pword)
		{
			std::cout << "\n" << label;
			std::getline(std::cin, var);

			return var;
		}
		else
		{
			var = "";
			std::cout << "\n" << label;
			while (true)
			{
				char kp = _getch();
				if (kp == VK_BACK)
				{
					if (var.length() < 1)
						continue;
					var.erase(var.size() - 1);
					std::cout << "\x1B[1D" << "\x1B[1P";
					continue;
				}
				if (kp == VK_RETURN)
				{
					return var;
				}
				else
				{
					var += kp;
					std::cout << '*';
				}

			}
		}
	}
	inline std::string reversestring(std::string content)
	{
		std::reverse(content.begin(), content.end());

		return content;
	}
	inline std::string replaceAll(std::string subject, const std::string& search,const std::string& replace) {
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos) {
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return subject;
	}
	inline std::string DownloadString(std::string URL) {
		HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		
		HINTERNET urlFile;
		std::string rtn;
		if (interwebs) {
			DWORD dwFlags = INTERNET_FLAG_NO_UI   // no UI please
				| INTERNET_FLAG_NO_AUTH           // don't authenticate
				| INTERNET_FLAG_PRAGMA_NOCACHE    // do not try the cache or proxy
				| INTERNET_FLAG_NO_CACHE_WRITE;   // don't add this to the IE cache
			urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, dwFlags, NULL);
			if (urlFile) {
				char buffer[2000];
				DWORD bytesRead;
				do {
					InternetReadFile(urlFile, buffer, 2000, &bytesRead);
					rtn.append(buffer, bytesRead);
					memset(buffer, 0, 2000);
				} while (bytesRead);
				InternetCloseHandle(interwebs);
				InternetCloseHandle(urlFile);
				std::string p = replaceAll(rtn, "|n", "\r\n");
				return p;
			}
		}
		InternetCloseHandle(interwebs);
		std::string p = replaceAll(rtn, "|n", "\r\n");
		return p;
	}
	inline void setClipboardText(const char* text) {
		int size = ::MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
		if (size < 0) {
			return;
		}

		if (::OpenClipboard(NULL)) {
			::EmptyClipboard();
			HGLOBAL hGlobal = ::GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, (size + 1) * sizeof(WCHAR));
			if (hGlobal != NULL) {
				LPWSTR lpszData = (LPWSTR)::GlobalLock(hGlobal);
				if (lpszData != nullptr) {
					::MultiByteToWideChar(CP_UTF8, 0, text, -1, lpszData, size);
					::GlobalUnlock(hGlobal);
					::SetClipboardData(CF_UNICODETEXT, hGlobal);
				}
			}
			::CloseClipboard();
		}
	}
	inline std::string GetCPUID() {
		std::array<int, 4> cpuinfo;
		__cpuid(cpuinfo.data(), 1);
		std::ostringstream buf;
		buf
			<< std::uppercase << std::hex << std::setfill('0')
			<< std::setw(8) << cpuinfo.at(3)
			<< std::setw(8) << cpuinfo.at(0);
		return buf.str();
	}
	inline std::string GetPCUser() {
		char   PCName[UNLEN + 1];
		DWORD  PCName_len = UNLEN + 1;
		GetUserName(PCName, &PCName_len);

		return PCName;
	}
}

//! termcolor
//! ~~~~~~~~~
//!
//! termcolor is a header-only c++ library for printing colored messages
//! to the terminal. Written just for fun with a help of the Force.
//!
//! :copyright: (c) 2013 by Ihor Kalnytskyi
//! :license: BSD, see LICENSE for details
//!

#ifndef TERMCOLOR_HPP_
#define TERMCOLOR_HPP_

// the following snippet of code detects the current OS and
// defines the appropriate macro that is used to wrap some
// platform specific things
#if defined(_WIN32) || defined(_WIN64)
#   define TERMCOLOR_OS_WINDOWS
#elif defined(__APPLE__)
#   define TERMCOLOR_OS_MACOS
#elif defined(__unix__) || defined(__unix)
#   define TERMCOLOR_OS_LINUX
#else
#   error unsupported platform
#endif


// This headers provides the `isatty()`/`fileno()` functions,
// which are used for testing whether a standart stream refers
// to the terminal. As for Windows, we also need WinApi funcs
// for changing colors attributes of the terminal.
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
#   include <unistd.h>
#elif defined(TERMCOLOR_OS_WINDOWS)
#   include <io.h>
#   include <windows.h>
#endif



namespace termcolor
{
	// Forward declaration of the `_internal` namespace.
	// All comments are below.
	namespace _internal
	{
		// An index to be used to access a private storage of I/O streams. See
		// colorize / nocolorize I/O manipulators for details.
		static int colorize_index = std::ios_base::xalloc();

		inline FILE* get_standard_stream(const std::ostream& stream);
		inline bool is_colorized(std::ostream& stream);
		inline bool is_atty(const std::ostream& stream);

#if defined(TERMCOLOR_OS_WINDOWS)
		inline void win_change_attributes(std::ostream& stream, int foreground, int background = -1);
#endif
	}

	inline
		std::ostream& colorize(std::ostream& stream)
	{
		stream.iword(_internal::colorize_index) = 1L;
		return stream;
	}

	inline
		std::ostream& nocolorize(std::ostream& stream)
	{
		stream.iword(_internal::colorize_index) = 0L;
		return stream;
	}

	inline
		std::ostream& reset(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[00m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream, -1, -1);
#endif
		}
		return stream;
	}


	inline
		std::ostream& bold(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[1m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
		}
		return stream;
	}


	inline
		std::ostream& dark(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[2m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
		}
		return stream;
	}


	inline
		std::ostream& underline(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[4m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
		}
		return stream;
	}


	inline
		std::ostream& blink(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[5m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
		}
		return stream;
	}


	inline
		std::ostream& reverse(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[7m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
		}
		return stream;
	}


	inline
		std::ostream& concealed(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[8m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
		}
		return stream;
	}


	inline
		std::ostream& grey(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[30m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream,
				0   // grey (black)
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& red(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[31m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream,
				FOREGROUND_RED
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& green(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[32m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream,
				FOREGROUND_GREEN
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& yellow(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[33m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream,
				FOREGROUND_GREEN | FOREGROUND_RED
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& blue(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[34m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream,
				FOREGROUND_BLUE
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& magenta(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[35m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream,
				FOREGROUND_BLUE | FOREGROUND_RED
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& cyan(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[36m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream,
				FOREGROUND_BLUE | FOREGROUND_GREEN
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& white(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[37m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream,
				FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
			);
#endif
		}
		return stream;
	}



	inline
		std::ostream& on_grey(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[40m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream, -1,
				0   // grey (black)
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& on_red(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[41m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream, -1,
				BACKGROUND_RED
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& on_green(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[42m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream, -1,
				BACKGROUND_GREEN
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& on_yellow(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[43m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream, -1,
				BACKGROUND_GREEN | BACKGROUND_RED
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& on_blue(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[44m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream, -1,
				BACKGROUND_BLUE
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& on_magenta(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[45m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream, -1,
				BACKGROUND_BLUE | BACKGROUND_RED
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& on_cyan(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[46m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream, -1,
				BACKGROUND_GREEN | BACKGROUND_BLUE
			);
#endif
		}
		return stream;
	}

	inline
		std::ostream& on_white(std::ostream& stream)
	{
		if (_internal::is_colorized(stream))
		{
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			stream << "\033[47m";
#elif defined(TERMCOLOR_OS_WINDOWS)
			_internal::win_change_attributes(stream, -1,
				BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED
			);
#endif
		}

		return stream;
	}



	//! Since C++ hasn't a way to hide something in the header from
	//! the outer access, I have to introduce this namespace which
	//! is used for internal purpose and should't be access from
	//! the user code.
	namespace _internal
	{
		//! Since C++ hasn't a true way to extract stream handler
		//! from the a given `std::ostream` object, I have to write
		//! this kind of hack.
		inline
			FILE* get_standard_stream(const std::ostream& stream)
		{
			if (&stream == &std::cout)
				return stdout;
			else if ((&stream == &std::cerr) || (&stream == &std::clog))
				return stderr;

			return 0;
		}

		// Say whether a given stream should be colorized or not. It's always
		// true for ATTY streams and may be true for streams marked with
		// colorize flag.
		inline
			bool is_colorized(std::ostream& stream)
		{
			return is_atty(stream) || static_cast<bool>(stream.iword(colorize_index));
		}

		//! Test whether a given `std::ostream` object refers to
		//! a terminal.
		inline
			bool is_atty(const std::ostream& stream)
		{
			FILE* std_stream = get_standard_stream(stream);

			// Unfortunately, fileno() ends with segmentation fault
			// if invalid file descriptor is passed. So we need to
			// handle this case gracefully and assume it's not a tty
			// if standard stream is not detected, and 0 is returned.
			if (!std_stream)
				return false;

#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
			return ::isatty(fileno(std_stream));
#elif defined(TERMCOLOR_OS_WINDOWS)
			return ::_isatty(_fileno(std_stream));
#endif
		}

#if defined(TERMCOLOR_OS_WINDOWS)
		//! Change Windows Terminal colors attribute. If some
		//! parameter is `-1` then attribute won't changed.
		inline void win_change_attributes(std::ostream& stream, int foreground, int background)
		{
			// yeah, i know.. it's ugly, it's windows.
			static WORD defaultAttributes = 0;

			// Windows doesn't have ANSI escape sequences and so we use special
			// API to change Terminal output color. That means we can't
			// manipulate colors by means of "std::stringstream" and hence
			// should do nothing in this case.
			if (!_internal::is_atty(stream))
				return;

			// get terminal handle
			HANDLE hTerminal = INVALID_HANDLE_VALUE;
			if (&stream == &std::cout)
				hTerminal = GetStdHandle(STD_OUTPUT_HANDLE);
			else if (&stream == &std::cerr)
				hTerminal = GetStdHandle(STD_ERROR_HANDLE);

			// save default terminal attributes if it unsaved
			if (!defaultAttributes)
			{
				CONSOLE_SCREEN_BUFFER_INFO info;
				if (!GetConsoleScreenBufferInfo(hTerminal, &info))
					return;
				defaultAttributes = info.wAttributes;
			}

			// restore all default settings
			if (foreground == -1 && background == -1)
			{
				SetConsoleTextAttribute(hTerminal, defaultAttributes);
				return;
			}

			// get current settings
			CONSOLE_SCREEN_BUFFER_INFO info;
			if (!GetConsoleScreenBufferInfo(hTerminal, &info))
				return;

			if (foreground != -1)
			{
				info.wAttributes &= ~(info.wAttributes & 0x0F);
				info.wAttributes |= static_cast<WORD>(foreground);
			}

			if (background != -1)
			{
				info.wAttributes &= ~(info.wAttributes & 0xF0);
				info.wAttributes |= static_cast<WORD>(background);
			}

			SetConsoleTextAttribute(hTerminal, info.wAttributes);
		}
#endif // TERMCOLOR_OS_WINDOWS

	} // namespace _internal

} // namespace termcolor


#undef TERMCOLOR_OS_WINDOWS
#undef TERMCOLOR_OS_MACOS
#undef TERMCOLOR_OS_LINUX

#endif // TERMCOLOR_HPP_