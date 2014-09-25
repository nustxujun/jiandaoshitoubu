#include "SkillLua.h"
#include "SkillContainer.h"
#include "LuaTypeConvertion.h"
#include "Entity.h"
#include "Environment.h"
#include "CommonFile.h"
#include "Skill.h"
#include "SkillResult.h"
#include "Resource.h"
#include "SkillEffect.h"
using namespace JSB;

SkillLua::SkillLua(SkillContainer* cont, const String& name, CommonFile* file)
	:SkillInstance(cont, name)
{
	using namespace luareg;

	{
		CommonFile* base = (CommonFile*)ResourceManager::getSingleton().getResource(STR("Types.lua"));
		if (base->getState() != RS_OK)
			JSB_EXCEPT("cannot find Types.lua ");

		Executer::execute(mState,
			(const char*)base->getData(),
			base->getSize(),
			base->getName().c_str());
	}

	{
		CommonFile* base = (CommonFile*)ResourceManager::getSingleton().getResource(STR("SkillEffective.lua"));
		if (base->getState() != RS_OK)
			JSB_EXCEPT("cannot find SkillEffective.lua ");

		Executer::execute(mState,
							(const char*)base->getData(),
							base->getSize(),
							base->getName().c_str());
	}

	{
		module(mState)
			<< def("setTriggerName", this, &SkillLua::setTriggerName)
			<< defLuaFunc("effect", this, &SkillLua::effect);

	}

	Executer::execute(mState,
					  (const char*)file->getData(),
					  file->getSize(),
					  file->getName().c_str());

	
}

SkillLua::~SkillLua()
{

}

const String& SkillLua::getTriggerName()
{
	return mTrigger;
}

void SkillLua::setTriggerName(const String& name)
{
	mTrigger = name;
}

int SkillLua::effect(luareg::state_t& state)
{
	int top = ::lua_gettop(state);
	if (top == 0)
		JSB_EXCEPT("effect func need parameter");

	int type = luareg::convertion_t<int>::from(state, 1);
	SkillEffect::Ptr se(SkillEffectFactory::createSkillEffect((SkillEffectType)type));
	se->parseParameter(state,2);
	mEffects.push_back(se);
	return 0;
}




//SkillResult SkillLua::trigger(Environment* env)
//{
//	try
//	{
//		return luareg::call(mState, "trigger", env);
//	}
//	catch (const luareg::fatal_error_t &e)
//	{
//		std::cout << e.what() << std::endl;
//		e.dump(std::cout);
//		return SkillResult();
//	}
//}

SkillInstance* SkillLuaConstructor::construct(SkillContainer* cont, const String& name)const
{
	Skill* skill = SkillManager::getSingleton().getSkill(name);
	return new SkillLua(cont, name, skill->getResFile());
}
