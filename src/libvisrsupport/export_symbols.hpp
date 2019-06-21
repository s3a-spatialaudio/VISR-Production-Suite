/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#ifndef VISRSUPPORT_EXPORT_SYMBOLS_HPP_INCLUDED
#define VISRSUPPORT_EXPORT_SYMBOLS_HPP_INCLUDED

/**
 * @file
 *
 */

#ifdef BUILD_VISRSUPPORT_SHARED_LIBRARY

#ifdef _MSC_VER // Windows platform
#ifdef BUILD_VISRSUPPORT_LIBRARY
#define VISRSUPPORT_LIBRARY_SYMBOL __declspec(dllexport)
#else
#define VISRSUPPORT_LIBRARY_SYMBOL __declspec(dllimport)
#endif // #ifdef VISR_BUILD_DAWSUPPORT_LIBRARIES
#elif defined(__GNUC__)

// Unix platforms (Linux and Mac OS X)
#define VISRSUPPORT_LIBRARY_SYMBOL __attribute__((visibility("default")))

#else
#error "Platform does not support symbol export."
#endif // #ifdef _WIN32

#else // BUILD_VISRSUPPORT_SHARED_LIBRARY

#define VISRSUPPORT_LIBRARY_SYMBOL // expand to empty string

#endif // BUILD_VISRSUPPORT_SHARED_LIBRARY

#endif // #ifndef VISRSUPPORT_EXPORT_SYMBOLS_HPP_INCLUDED
