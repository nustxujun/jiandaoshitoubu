#ifndef _Profile_H_
#define _Profile_H_

#include "Resource.h"
#include "Property.h"
#include <fstream>

namespace JSB
{
	class Profile:public Resource
	{
	public:
		static const Char* RESOURCE_TYPE;

		typedef std::vector<String> StringVec;
	public:
		Profile(const String& filename, const CustomParams* para);
		~Profile();

		String getType()const;
		const void* getData()const;
		size_t getSize()const;

		void save(const char* buffer, size_t size);
	public:
		std::vector<char> mBuffer;
		String mPath;

	};


	class ProfileFactory : public ResourceFactory
	{
	public:
		Resource* createImpl(const String& name, const CustomParams* paras)
		{
			return new Profile(name, paras);
		}

		void destroyImpl(Resource* res)
		{
			delete (Profile*)res;
		}

	};
}

#endif