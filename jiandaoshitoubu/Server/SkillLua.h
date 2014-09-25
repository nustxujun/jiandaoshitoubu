#ifndef _SkillLua_H_
#define _SkillLua_H_

#include "SkillInstance.h"
#include "lua_reg.hpp"
#include "LuaTypeConvertion.h"
#include <memory>
#include "SkillEffect.h"

namespace JSB
{
	
	class SkillLua:public SkillInstance
	{
	public:
		SkillLua(SkillContainer* cont, const String& name, CommonFile* file);
		~SkillLua();

		const String& getTriggerName();

		template<class ... Args>
		Effects trigger(Args ... args)
		{
			try
			{
				luareg::call(mState, "trigger", args...);
			}
			catch (const luareg::fatal_error_t &e)
			{
				std::cout << e.what() << std::endl;
				e.dump(std::cout);
			}

			Effects tmp;
			tmp.swap(mEffects);
			return tmp;
		}


	private:
		void setTriggerName(const String& name);
		int effect(luareg::state_t& state);

	private:
		luareg::state_t mState;
		String mTrigger;
		Effects mEffects;
	};

	class SkillLuaConstructor:public SkillInstanceConstructor
	{
	public:
		SkillInstance* construct(SkillContainer* cont, const String& name)const;

	};
}

#endif