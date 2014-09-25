#ifndef _SkillTrigger_H_
#define _SkillTrigger_H_

#include "SkillInstance.h"

namespace JSB
{
	class SkillTrigger
	{
	public :
		typedef std::set<SkillInstance*> Skills;
	public:
		virtual void addSkill(SkillInstance* skill);
		virtual void removeSkill(SkillInstance* skill);
		virtual void clear();
		virtual Skills& getSkills();

	protected:

	private:
		Skills mSkills;
	};
}


#endif