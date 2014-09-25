#ifndef _LogicType_H_
#define _LogicType_H_

namespace JSB
{
	enum TargetType
	{
		TT_NONE,
		TT_SELF	=		1 ,
		TT_PARTNER =	1 << 1,
		TT_ENEMY =		1 << 2,
		TT_SINGLE =		1 << 3,
		TT_TEAM	=		1 << 4,
	};


	enum SkillEffectType
	{
		SET_NORMALATTACK,

	};

}

#endif