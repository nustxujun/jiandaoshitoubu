#include "Mod.h"
#include "FindFile.h"
#include "Resource.h"
#include "DataStream.h"
#include "Events.h"
#include "User.h"
#include "CommonFile.h"
#include "Skill.h"
#include "Monster.h"

using namespace JSB;

Mod::Mod(const String& name, const String& dir)
{
	FindFile ff;
	ResourceManager& resmgr = ResourceManager::getSingleton();


	auto fl = ff.find(dir + STR("\\*.*"), FT_DIR, 0);
	for (auto i : fl)
	{
		ResourceType type = getType(i.name);

		auto l = ff.find(i.full + STR("\\*.*"), FT_FILE,0);
		for (auto j : l)
		{
			CustomParams para;
			para[STR("path")] = j.full;
			CommonFile* res = (CommonFile*)resmgr.createResource(
				j.name, CommonFile::RESOURCE_TYPE, &para);

			addResource(type, res);

		}
	}


	load();

}

Mod::~Mod()
{
}

void Mod::addResource(ResourceType type, CommonFile* res)
{
	mRes[type].push_back(res);
}

Mod::ResourceType Mod::getType(const String& name)
{
	if (name == STR("skill"))
		return SKILL;
	else if (name == STR("monster"))
		return MONSTER;
	else
		return OTHER;

}

void Mod::load()
{
	SkillManager& sm = SkillManager::getSingleton();
	for (auto i : mRes[SKILL])
	{
		const String& name = i->getName();
		sm.createSkill(String(name.c_str(), name.find('.')), i);
	}


	MonsterFactory& mf = MonsterFactory::getSingleton();
	for (auto i : mRes[MONSTER])
	{
		const String& name = i->getName();
		mf.add(i);
	}

}

void Mod::unload()
{
}


