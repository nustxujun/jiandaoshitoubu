#include "SkillEffect.h"
#include "DataStream.h"
#include "User.h"

using namespace JSB;

SkillEffect* SkillEffectFactory::createSkillEffect(SkillEffectType type)
{
	switch (type)
	{
	case SET_NORMALATTACK:
		return new NormalAttack();
	default:
		JSB_EXCEPT("unknown skilleffect type");
	}
}

NormalAttack::NormalAttack()
	:SkillEffect(SET_NORMALATTACK)
{
	
}

void NormalAttack::parseParameter(luareg::state_t& state, int offset)
{
	int top = ::lua_gettop(state);
	if ((top - offset) < 0) JSB_EXCEPT("parameter conut is wrong");

	setTarget(luareg::convertion_t<size_t>::from(state, offset++));

}

void NormalAttack::serialize(DataStream& data)
{
	data << getType();
	data << mTarget;
	data << mDamage;
}

void NormalAttack::effect(ServerEntity* caster, ServerEntity* target)
{
	const Property& cp = caster->getProperty();
	const Property& tp = target->getProperty();

	mDamage = cp.str;
	mTarget = target->getID();

	target->hurt(getType(), mDamage);

}