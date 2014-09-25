#include "CommonFile.h"
#include <fstream>
using namespace JSB;

const Char* CommonFile::RESOURCE_TYPE = STR("CommonFile");

CommonFile::CommonFile(const String& filename, const CustomParams* para)
{
	mName = filename;
	if (para == nullptr) return;
	
	auto path = para->find(STR("path"));
	if (path == para->end()) return;

	std::ifstream f(path->second, std::ios::in | std::ios::binary);
	if (!f)
	{
		setState(RS_FILE_NOT_EXISTED);
		return;
	}
	f.seekg(0, std::ios_base::end);
	mSize = f.tellg();
	f.seekg(0, std::ios_base::beg);

	mBuffer = new char[mSize];
	f.read(mBuffer, mSize);
}

CommonFile::~CommonFile()
{
	if (mBuffer != nullptr)
		delete mBuffer;
}

void* CommonFile::getData()
{
	return mBuffer;
}

String CommonFile::getType()const
{
	return RESOURCE_TYPE;
}

String CommonFile::getName()const
{
	return mName;
}


size_t CommonFile::getSize()const
{
	return mSize;
}
