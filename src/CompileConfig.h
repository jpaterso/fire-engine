/**
 * FILE:    CompileConfig.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: CompileConfig.h 119 2007-12-03 02:12:08Z jpaterso $
 * PURPOSE: Compile time options -- library exporting, debug switches,
 *          optional compilation etc.
**/

#ifndef __COMPILECONFIG_H_INCLUDED__
#define __COMPILECONFIG_H_INCLUDED__

#if defined(_WIN32)
// We are on a Win32 system
#	define _FIRE_ENGINE_WIN32_
// Skip some of the more exotic headers, and make compilation faster
#	define WIN32_LEAN_AND_MEAN
#	if defined(_MSC_VER)
#		if defined(__FIRE_ENGINE_BUILDING_LIB__)
#			define _FIRE_ENGINE_API_ __declspec(dllexport)
#		else
#			define _FIRE_ENGINE_API_ __declspec(dllimport)
#		endif
#	else
#		define _FIRE_ENGINE_API_
#	endif // _MSC_VER
#else
#	define _FIRE_ENGINE_API_
#endif // _WIN32

#if defined(_MSC_VER)
//#	pragma warning (disable:4251) // disable annoying import/export warning in VC++
#	pragma warning (disable:4661) // disable some warnings related to static member variables in templates
#endif

#if defined(__APPLE__) || defined(Macintosh)
#	define _FIRE_ENGINE_BIG_ENDIAN_
#else
#	define _FIRE_ENGINE_LITTLE_ENDIAN_
#endif

/** Mouse events are generated very often, especially the mouse dragged event. Uncomment
 this to enable the generation of mouse dragged events. */
//#define _FIRE_ENGINE_GENERATE_MOUSE_MOVED_EVENTS_

/**
 *	Compile options. Valid options are:
 *  _FIRE_ENGINE_DEBUG_ALL_:         Activate debugging information for everything
 *      _FIRE_ENGINE_DEBUG_BMP_:     Activate debugging information for BMP image loading
 *      _FIRE_ENGINE_DEBUG_TGA_:     Activate debugging information for TGA image loading
 *      _FIRE_ENGINE_DEBUG_PCX_:     Activate debugging information for PCX image loading
 *      _FIRE_ENGINE_DEBUG_OPENGL_:  Activate debugging information for OpenGL rendering
 *      _FIRE_ENGINE_DEBUG_3DS_:     Activate debugging information for 3DSMax loading
 *      _FIRE_ENGINE_DEBUG_MD2_:     Activate debugging information for MD2 Mesh loading
 *      _FIRE_ENGINE_DEBUG_MD3_:     Activate debugging information for MD3 model loading
 *      _FIRE_ENGINE_DEBUG_OBJECT_:  Add a debug name to fire engine's base objects
 *      _FIRE_ENGINE_DEBUG_ZIP_:     Activate debugging information when loading ZIP files
 *      _FIRE_ENGINE_DEBUG_MEMORY_:  Debug memory - check for memory leaks and so on.
 *
 *  _FIRE_ENGINE_COMPILE_WITH_OPENGL_:       Compile using the OpenGL library
**/
#define	_FIRE_ENGINE_COMPILE_WITH_OPENGL_

#if defined(DEBUG)
#	define _FIRE_ENGINE_DEBUG_ALL_
#endif

#if defined(_FIRE_ENGINE_DEBUG_ALL_)
#	define _FIRE_ENGINE_DEBUG_BMP_
#	define _FIRE_ENGINE_DEBUG_TGA_
#	define _FIRE_ENGINE_DEBUG_PCX_
#	define _FIRE_ENGINE_DEBUG_OPENGL_
#	define _FIRE_ENGINE_DEBUG_3DS_
#	define _FIRE_ENGINE_DEBUG_MD2_
#	define _FIRE_ENGINE_DEBUG_MD3_
#	define _FIRE_ENGINE_DEBUG_OBJECT_
#   define _FIRE_ENGINE_DEBUG_ZIP_
#endif

#if defined(_FIRE_ENGINE_DEBUG_MEMORY_)
#	include "MemoryManager.h"
#endif

#endif // __COMPILECONFIG_H_INCLUDED__
