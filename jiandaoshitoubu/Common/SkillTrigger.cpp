#include "SkillTrigger.h"

using namespace JSB;

void SkillTrigger::addSkill(SkillInstance* skill)
{
	if (!mSkills.insert(skill).second)
		return;
}

void SkillTrigger::removeSkill(SkillInstance* skill)
{
	auto ret = mSkills.find(skill);
	if (ret == mSkills.end()) return;
}

SkillTrigger::Skills& SkillTrigger::getSkills()
{
	return mSkills;
}

void SkillTrigger::clear()
{
	mSkills.clear();
}

