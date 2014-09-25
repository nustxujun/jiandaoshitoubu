#ifndef _UserDataSerializer_H_
#define _UserDataSerializer_H_

#include "EntityDataSerializer.h"
#include "SkillLua.h"

namespace JSB
{
	class UserDataSerializer :public EntityDataSerializer
	{

		void parseProp(DataStream& s, Entity* entity)
		{
			Property property;
			parse(s, property.name);
			parse(s, property.hp);
			parse(s, property.str);
			parse(s, property.dex);
			parse(s, property.inte);
			parse(s, property.speed);
			parse(s, property.level);
			parse(s, property.exp);
			parse(s, property.progress);

			parseEnd(s);

			entity->setProperty(property);

		}

		void parseSkill(DataStream& s, Entity* entity)
		{
			String skill;
			parse(s, skill);
			parseEnd(s);

			SkillLuaConstructor cont;
			entity->getSkillContainer().addSkill(skill, cont);
		}

	};
}

#endif