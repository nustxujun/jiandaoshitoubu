#ifndef _CommonText_H_
#define _CommonText_H_

#include "Common.h"
#include "Resource.h"
namespace JSB
{
	class CommonFile:public Resource
	{
	public:
		static const Char* RESOURCE_TYPE;

	public:
		CommonFile(const String& filename, const CustomParams* para);
		~CommonFile();

		void* getData();

		String getType()const;
		size_t getSize()const;
		String getName()const;
	private:
		char* mBuffer = 0;
		size_t mSize = 0;
		String mName;
	};

	class CommonFileFactory : public ResourceFactory
	{
	public:
		Resource* createImpl(const String& name, const CustomParams* paras)
		{
			return new CommonFile(name, paras);
		}

		void destroyImpl(Resource* res)
		{
			delete (CommonFile*)res;
		}

	};

}

#endif