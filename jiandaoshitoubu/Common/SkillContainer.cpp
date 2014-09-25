#include "SkillContainer.h"
#include "SkillInstance.h"
#include "SkillResult.h"
#include "SkillTrigger.h"
using namespace JSB;

SkillContainer::SkillContainer(Entity* caster) :mCaster(caster)
{

}

SkillContainer::~SkillContainer()
{
	clear();

	for (auto i : mTriggers)
	{
		delete i.second;
	}
	mTriggers.clear();
}


void SkillContainer::addSkill(const String& name, const SkillInstanceConstructor & cont)
{
	auto ret = mSkills.insert(std::make_pair(name, nullptr));
	if (!ret.second)
		JSB_EXCEPT("same skill is existed");

	SkillInstance* inst = cont.construct(this, name);
	ret.first->second = inst;


	SkillTrigger* trigger = getTrigger(inst->getTriggerName()); 
	trigger->addSkill(inst);

}

void SkillContainer::removeSkill(const String& name)
{
	auto ret = mSkills.find(name);
	if (ret == mSkills.end()) return;

	delete ret->second;
	mSkills.erase(ret);
}

bool SkillContainer::hasSkill(const String& name)
{
	return mSkills.find(name) != mSkills.end();
}

SkillContainer::Skills SkillContainer::getSkills()
{
	return Skills(mSkills.begin(), mSkills.end());
}

size_t SkillContainer::getSkillCount()const
{
	return mSkills.size();
}

void SkillContainer::clear()
{
	

	for (auto i : mSkills)
	{
		delete i.second;
	}
	mSkills.clear();

	for (auto i : mTriggers)
	{
		i.second->clear();
	}
}

SkillTrigger* SkillContainer::getTrigger(const String& name)
{
	auto ret = mTriggers.find(name);
	if (ret == mTriggers.end())
		JSB_EXCEPT("specify trigger is not existed");

	return ret->second;
}

Entity* SkillContainer::getCaster()
{
	return mCaster;
}
