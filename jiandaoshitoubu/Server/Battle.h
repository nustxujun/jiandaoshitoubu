#ifndef _Battle_H_
#define _Battle_H_

#include "Common.h"
#include "Monster.h"
#include "lua_reg.hpp"

namespace JSB
{
	class Battle
	{
	public:
		Battle(SceneNode* root);
		void update(float elapsedTime);
		void init();
	private:
		void action(float elapsedTime);
		void afterbattle();
		void generatMonster();

		template<class R, class ... P>
		R callLua(const String& func, P... p)
		{
			return luareg::call(mLuaState, func.c_str(), p...);
		}

		void print(User* user, const String& str);

	private:

		SceneNode* mNode ;
		Monster mMonster ;
		std::vector<String> mMonsterTemplates;

		enum State
		{
			WAIT,
			BEGIN,
			FIGHT,
			END,
		};

		State mState = WAIT;

		luareg::state_t mLuaState;

	};

}

#endif