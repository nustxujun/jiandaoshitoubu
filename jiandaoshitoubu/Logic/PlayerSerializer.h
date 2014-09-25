#ifndef _PlayerSerializer_H_
#define _PlayerSerializer_H_

#include "EntityDataSerializer.h"
#include "SkillShell.h"
namespace JSB
{
	class PlayerSerializer : public EntityDataSerializer
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

			SkillShellCont cont;
			SkillContainer& c = entity->getSkillContainer();
			if (!c.hasSkill(skill))
				c.addSkill(skill, cont);
		}
	}ps;
}
#endif