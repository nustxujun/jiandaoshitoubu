#include "FindFile.h"
#include <Windows.h>
using namespace JSB;

FindFile::FileList FindFile::find(const String& file, FileType type, int depth)
{
	FileList fl;
	find(fl, file, type, depth);
	return fl;
}

void FindFile::find(FileList& fl, const String& file, FileType type, int depth )
{
	Char dev[2] = { '\\', 0 };

	String path(file.c_str(), file.rfind(dev));
	String name(file.c_str() + file.rfind(dev) + 1);

	WIN32_FIND_DATA data;
	HANDLE h = ::FindFirstFile(file.c_str(), &data);
	if (h == INVALID_HANDLE_VALUE)
		return ;

	do
	{

		if (data.cFileName[0] == '.')
		{
			if (data.cFileName[1] == 0)
				continue;
			if (data.cFileName[1] == '.' &&
				data.cFileName[2] == 0)
				continue;


		}

		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (depth != 0)
				find(fl, path + dev + data.cFileName + dev + name, type, depth - 1);

			if (type == FT_FILE)
				continue;
		}
		else if (type == FT_DIR)
			continue;

		fl.push_back(File(data.cFileName, path + dev + data.cFileName));

	} while (::FindNextFile(h, &data));


	::FindClose(h);

	return ;
}
