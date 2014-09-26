#include "Monster.h"
#include "Entity.h"
#include "UserDataSerializer.h"
#include "CommonFile.h"
#include "SkillLua.h"
#include "Triggers.h"
#include "Environment.h"
#include "SceneNode.h"

using namespace JSB;

Monster::Monster(SceneNode* node) :ServerEntity(0), mNode(node)
{
	Triggers::generatorTriggers(&mSkillCont);
}

void Monster::serialize(DataStream& data)
{
}

void Monster::unserialize(DataStream& data)
{
	UserDataSerializer us;
	us.unserialize(data, this);
}

void Monster::useSkill(Environment& env)
{
	RandomTrigger* rt = (RandomTrigger*)mSkillCont.getTrigger(Triggers::RANDOM_TRIGGER);
	auto skill = rt->trigger(env);
	if (skill == nullptr) return;
	auto es = skill->trigger();
	for (auto& i : es)
	{
		for (auto &j : env.getTarget(i->getTarget()))
		{
			i->effect(env.getVisitor(), j);
		}
	}

}

void Monster::broadcast(DataStream& msg)
{
	mNode->broadcast(msg);
}


void Monster::hurt(SkillEffectType type, int damage)
{
	mProperty.hp = std::max(0, mProperty.hp - damage);
}


void MonsterFactory::add(CommonFile* file)
{
	Monster* m = new Monster(nullptr);
	DataStream data(file->getData(), file->getSize());
	m->unserialize(data);
	const String& name = m->getProperty().name;

	auto ret = mTemplates.insert(std::make_pair(name, m));
	if (!ret.second)
		JSB_EXCEPT("same monster is existed");

}

void MonsterFactory::generate(const String& name, Monster& m)
{
	auto ret = mTemplates.find(name);
	if (ret == mTemplates.end())
		JSB_EXCEPT("specify monster is not existed");

	auto tpl = ret->second;
	m.getPropertyRef() = tpl->getProperty();
	SkillLuaConstructor cont;
	SkillContainer& sc = m.getSkillContainer();
	sc.clear();
	for (auto i : tpl->getSkillContainer().getSkills())
	{
		sc.addSkill(i.first, cont);
	}
}

MonsterFactory::TemplateIterator MonsterFactory::getTemplates()
{
	return TemplateIterator(mTemplates.begin(), mTemplates.end());
}