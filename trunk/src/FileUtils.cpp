/**
 * RCS ID:  $Id$
**/

#include "FileUtils.h"
#include "string.h"
#include <string.h>

namespace fire_engine
{
namespace io
{

string FileUtils::StripDirectory(const string& filename)
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
	return string(stripped);
}

string FileUtils::StripExtension(const fire_engine::string &filename)
{
	const c8 * fn = filename.c_str();
	const c8 * dotOffset;
	if ((dotOffset = strrchr(fn, '.')) != nullptr)
	{
		c8 * stripped = new c8[dotOffset-fn+1];
		memcpy(stripped, fn, dotOffset-fn);
		stripped[dotOffset-fn] = '\0';
		return string(stripped, true);
	}
	return filename;
}

string FileUtils::GetFileExtension(const string& filename)
{
	const c8 * occ = strrchr(filename.c_str(), '.');
	if (occ == nullptr)
	{
		return string("");
	}
	return string(occ+1);
}

} //namespace io
} //namespace fire_engine
