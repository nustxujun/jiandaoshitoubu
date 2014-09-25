#ifndef _SkillInstance_H_
#define _SkillInstance_H_

#include "Common.h"
#include "SkillResult.h"

namespace JSB
{
	class SkillInstance
	{
	public:
		SkillInstance(SkillContainer* cont, const String& name);
		virtual ~SkillInstance();

		virtual const String& getTriggerName() = 0;
		const String& getName()const;
	protected:
		String mName;
		SkillContainer* mContainer;
	};

	class SkillInstanceConstructor
	{
	public:
		virtual SkillInstance* construct(SkillContainer* cont, const String& name)const = 0;

	};
}


#endif