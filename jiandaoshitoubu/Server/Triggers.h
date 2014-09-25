#ifndef _Triggers_H_
#define _Triggers_H_

#include "Common.h"
#include "SkillTrigger.h"
#include "SkillEffect.h"
namespace JSB
{
	class Triggers
	{
	public :
		static const Char* RANDOM_TRIGGER;
		static void generatorTriggers(SkillContainer* cont); 

	};


	class RandomTrigger :public SkillTrigger
	{
	public:
		SkillLua* trigger(Environment& env);
		void clear();
		void addSkill(SkillInstance* skill);
		void removeSkill(SkillInstance* skill);

	private:


	private:
		std::vector<SkillInstance*> mPri;

	};
}

#endif