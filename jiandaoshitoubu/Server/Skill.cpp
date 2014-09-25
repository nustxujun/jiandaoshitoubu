#include "Skill.h"
#include "Environment.h"

using namespace JSB;


Skill::Skill(const String& name,CommonFile* file):
	mRes(file), mName(name)
{


}

Skill::~Skill()
{
}


CommonFile* Skill::getResFile()
{
	return mRes;
}

const String& Skill::getName()
{
	return mName;
}





Skill* SkillManager::createSkill(const String& name, CommonFile* file)
{
	auto ret = mSkills.insert(std::make_pair(name, nullptr));
	if (ret.second)
	{
		ret.first->second = new Skill(name, file);
	}
	else
	{
		JSB_EXCEPT("skill is existed.");
		return nullptr;
	}

	return ret.first->second;
}

Skill* SkillManager::getSkill(const String& name)
{
	auto ret = mSkills.find(name);
	if (ret == mSkills.end())
	{
		JSB_EXCEPT("skill is not existed.");
		return nullptr;
	}
	return ret->second;
}

SkillManager::Skills SkillManager::getSkills()
{
	return Skills(mSkills.begin(), mSkills.end());
}

size_t SkillManager::getSkillCount()const
{
	return mSkills.size();
}

