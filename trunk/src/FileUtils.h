/**
 * RCS ID:  $Id$
**/

#ifndef FILEUTILS_H_INCLUDED
#define FILEUTILS_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"

namespace fire_engine
{

class string;

namespace io
{

/** A class containing utility methods on files and filenames. */
class _FIRE_ENGINE_API_ FileUtils
{
public:
	/** Returns the base name for a given file, without any directory information.
	 The method will work for both unix style (/) and windows (\) style directory
	 delimiting conventions.
	 \param filename The full name of the file to be stripped.
	 \return         The stripped name of the file. */
	static string StripDirectory(const string& filename);

	/** Returns the name of a file without it's ending extension.
	 \param filename The name of the file to cut the extension off.
	 \return         The filename stripped of it's extension. */
	static string StripExtension(const string& filename);

	/** Returns the extension of a file, without the leading dot.
	 \param filename The full name of the file.
	 \return         The extension of the file, or an empty string if the file has
	                 no extension. */ 
	static string GetFileExtension(const string& filename);
};

} //namespace io
} //namespace fire_engine

#endif // FILEUTILS_H_INCLUDED
