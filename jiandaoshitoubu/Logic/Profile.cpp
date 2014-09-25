#include "Profile.h"
#include <fstream>
#include <windows.h>
using namespace JSB;
using namespace std;

const Char* Profile::RESOURCE_TYPE = STR("Profile");

Profile::Profile(const String& filename, const CustomParams* para)
{
	if (para == nullptr) return;

	auto path = para->find(STR("path"));
	if (path == para->end()) return;

	mPath = path->second;

	ifstream f(path->second, ios::in | ios::binary);
	if (!f)
	{
		setState(RS_FILE_NOT_EXISTED);
		return;
	}

	f.seekg(0, std::ios_base::end);
	size_t size = f.tellg();
	f.seekg(0, std::ios_base::beg);

	struct Header
	{
		int id;
	} header;

	f.read((char*)&header, sizeof(header));
	if (f.gcount() < sizeof(header))
	{
		setState(RS_FILE_FORMAT_INVALIED);
		return;
	}

	if (header.id != *(int*)"SAVE")
	{
		setState(RS_FILE_FORMAT_INVALIED);
		return;
	}

	size -= sizeof(Header);

	mBuffer.resize(size);
	f.read(&mBuffer[0], size);

}

Profile::~Profile()
{

}

const void* Profile::getData()const
{
	return mBuffer.data();
}

size_t Profile::getSize()const
{
	return mBuffer.size();
}

String Profile::getType()const
{
	return RESOURCE_TYPE;
}

void Profile::save(const char* buffer, size_t size)
{
	if (mPath.empty())
	{
		JSB_EXCEPT("no save file");
	}
	mBuffer.resize(size);
	memcpy(&mBuffer[0], buffer, size);

	ofstream o(mPath, ios::binary );
	
	Assert("存档文件被其他程序占用，无法保存" && !!o);

	struct Header
	{
		int id;
	} header;

	header.id = *(int*)"SAVE";
	o.write( (const char*)&header, sizeof(header));

	o.write(buffer, size);
}
