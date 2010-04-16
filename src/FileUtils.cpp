/**
 * RCS ID:  $Id$
**/

#include "FileUtils.h"
#include "String.h"
#include <String.h>

namespace fire_engine
{
namespace io
{

String FileUtils::StripDirectory(const String& filename)
{
	const c8 * fn = filename.c_str();
	const c8 * stripped;
	if ((stripped = strrchr(fn, '/')) != nullptr)
	{
		stripped++;
	}
	else if ((stripped = strrchr(fn, '\\')) != nullptr)
	{
		stripped++;
	}
	else
	{
		stripped = fn;
	}
	return String(stripped);
}

String FileUtils::StripExtension(const fire_engine::String &filename)
{
	const c8 * fn = filename.c_str();
	const c8 * dotOffset;
	if ((dotOffset = strrchr(fn, '.')) != nullptr)
	{
		c8 * stripped = new c8[dotOffset-fn+1];
		memcpy(stripped, fn, dotOffset-fn);
		stripped[dotOffset-fn] = '\0';
		return String(stripped, true);
	}
	return filename;
}

String FileUtils::GetFileExtension(const String& filename)
{
	const c8 * occ = strrchr(filename.c_str(), '.');
	if (occ == nullptr)
	{
		return String("");
	}
	return String(occ+1);
}

void FileUtils::ConvertPath(String& path)
{
	path.replaceAll('\\', '/');
}

} //namespace io
} //namespace fire_engine
