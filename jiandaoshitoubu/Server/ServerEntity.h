#ifndef _ServerEntity_H_
#define _ServerEntity_H_

#include "Entity.h"

namespace JSB
{
	class ServerEntity :public Entity
	{
	public:
		ServerEntity(Identity id);

		virtual void useSkill(Environment& env) = 0;
		virtual void hurt(SkillEffectType type, int damage) = 0;

	};
}

#endif