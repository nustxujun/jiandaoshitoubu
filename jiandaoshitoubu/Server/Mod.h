#ifndef _Mod_H_
#define _Mod_H_

#include "Common.h"
#include "DataStream.h"

namespace JSB
{
	class Mod
	{
	public:
		Mod(const String& name, const String& dir);
		~Mod();

		void load();
		void unload();

	private:
		enum ResourceType
		{
			SKILL,
			MONSTER,

			OTHER,
			TYPE_NUM
		};

		void addResource(ResourceType type, CommonFile* res);
		ResourceType getType(const String& name);
	private:
		typedef std::vector<CommonFile*> ResVec;

		ResVec mRes[TYPE_NUM];
	
	};
}

#endif