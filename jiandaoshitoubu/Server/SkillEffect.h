#ifndef _SkillEffect_H_
#define _SkillEffect_H_

#include "lua_reg.hpp"
#include "LogicType.h"
#include "Common.h"
#include <memory>

namespace JSB
{

	class SkillEffect
	{
	public:
		typedef std::shared_ptr<SkillEffect> Ptr;
	public: 
		SkillEffect(SkillEffectType type) :mType(type), mTarget(TT_SELF){}
		virtual ~SkillEffect(){}

		SkillEffectType getType()const { return mType; }

		void setTarget(size_t target){ mTarget = target; }
		size_t getTarget()const{ return mTarget; }

		virtual void parseParameter(luareg::state_t &state, int offset) = 0;
		virtual void serialize(DataStream& data) = 0;
		virtual void effect(ServerEntity* caster, ServerEntity* target) = 0;
	private:
		SkillEffectType mType;
		size_t	mTarget;
	};

	typedef std::vector<std::shared_ptr<SkillEffect>> Effects;


	class SkillEffectFactory
	{
	public:
		static SkillEffect* createSkillEffect(SkillEffectType type);

	};

	class NormalAttack:public SkillEffect
	{
	public:
		NormalAttack();

		void parseParameter(luareg::state_t &state, int offset);
		void serialize(DataStream& data);
		void effect(ServerEntity* caster, ServerEntity* target);

	private:
		int mDamage;
		Identity mTarget;
	};

}

#endif