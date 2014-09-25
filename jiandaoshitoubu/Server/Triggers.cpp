#include "Triggers.h"
#include "SkillContainer.h"
#include "SkillLua.h"
#include "Environment.h"
#include "SkillEffect.h"
#include "ServerEntity.h"
#include "Events.h"

using namespace JSB;

const Char* Triggers::RANDOM_TRIGGER = STR("RandomTrigger");

void Triggers::generatorTriggers(SkillContainer* cont)
{
	cont->createTrigger<RandomTrigger>(RANDOM_TRIGGER);
}


SkillLua* RandomTrigger::trigger(Environment& env)
{
	auto& skills = getSkills();
	
	if (skills.empty())
		return nullptr;
	
	SkillLua* s = (SkillLua*)(*skills.begin());

	return s;
}


void RandomTrigger::addSkill(SkillInstance* skill)
{
	SkillTrigger::addSkill(skill);
	mPri.push_back(skill);
}

void RandomTrigger::removeSkill(SkillInstance* skill)
{
	SkillTrigger::removeSkill(skill);
	auto i = mPri.begin();
	auto endi = mPri.end();
	for (; i != endi; ++i)
	{
		if (*i != skill)
			continue;

		mPri.erase(i);
		return;
	}
}

void RandomTrigger::clear()
{
	SkillTrigger::clear();
	mPri.clear();
}