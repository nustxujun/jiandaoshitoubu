#ifndef _FindFile_H_
#define _FindFile_H_

#include "Common.h"

namespace JSB
{
	enum FileType
 	{
		FT_FILE,
		FT_DIR,
		FT_ALL,

	};
	class FindFile
	{
	public:
		struct File
		{
			String name;
			String full;

			File(const String& n, const String& f):
				name(n), full(f)
			{}
		};
		typedef std::vector<File> FileList;
		static const int DEFAULT_DEPTH = 5;
	public :
		FileList find(const String& file, FileType type, int depth = DEFAULT_DEPTH);

	private:
		void find(FileList& fl, const String& file, FileType type, int depth = DEFAULT_DEPTH);
		
	};

}

#endif