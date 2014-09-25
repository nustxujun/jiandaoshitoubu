#ifndef _SkillShell_H_
#define _SkillShell_H_

#include "SkillInstance.h"

namespace JSB
{
	class SkillShell :public SkillInstance
	{
	public:
		SkillShell(SkillContainer* cont, const String& name) :SkillInstance(cont, name)
		{}
		
		const String& getTriggerName()
		{
			return mTrigger;
		}

		String mTrigger = { STR("shell_trigger") };
	};


	class SkillShellCont :public SkillInstanceConstructor
	{
	public:
		SkillInstance* construct(SkillContainer* cont, const String& name)const
		{
			return new SkillShell(cont, name);
		}

	};
}

#endif