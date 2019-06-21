/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#ifndef CORE_EXPORT_SYMBOLS_HPP_INCLUDED
#define CORE_EXPORT_SYMBOLS_HPP_INCLUDED

/**
 * @file
 *
 */

#ifdef BUILD_CORE_SHARED_LIBRARY

#ifdef _MSC_VER // Windows platform
#ifdef BUILD_CORE_LIBRARY
#define CORE_LIBRARY_SYMBOL __declspec(dllexport)
#else
#define CORE_LIBRARY_SYMBOL __declspec(dllimport)
#endif // #ifdef BUILD_CORE_LIBRARIES
#elif defined(__GNUC__)

// Unix platforms (Linux and Mac OS X)
#define CORE_LIBRARY_SYMBOL __attribute__((visibility("default"))) 

#else
#error "Platform does not support symbol export."
#endif // #ifdef _WIN32

#else // BUILD_CORE_SHARED_LIBRARY

#define CORE_LIBRARY_SYMBOL // expand to empty string

#endif // BUILD_CORE_SHARED_LIBRARY

#endif // #ifndef CORE_EXPORT_SYMBOLS_HPP_INCLUDED
