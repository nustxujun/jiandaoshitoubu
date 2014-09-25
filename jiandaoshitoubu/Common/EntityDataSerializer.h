#ifndef _EntityDataSerializer_H_
#define _EntityDataSerializer_H_

#include "DataStream.h"
#include "Property.h"
#include "Entity.h"
//#include "SkillLua.h"

namespace JSB
{ 

	class EntityDataSerializer
	{
	public:
		enum ID
		{
			ID_PROP,
			ID_SKILL,

			ID_END,
			ID_ERROR = ~0
		};

		union Keyword
		{
			char str[5];
			int val;
		};

		static const Keyword* getKeyword(int index)
		{
			static Keyword keywords[] =
			{
				"PROP",
				"SKIL",
				"END_"
			};

			return (index < sizeof(keywords)) ? keywords + index : nullptr;
		}

		void serialize(DataStream& s, Entity* entity)
		{
			const Property& property = entity->getProperty();

			s << getKeyword(ID_PROP)->val;
			s << property.name;
			s << property.hp;
			s << property.str;
			s << property.dex;
			s << property.inte;
			s << property.speed;
			s << property.level;
			s << property.exp;
			s << property.progress;
			s << getKeyword(ID_END)->val;

			for (auto i : entity->getSkillContainer().getSkills())
			{
				s << getKeyword(ID_SKILL)->val;
				s << i.first;
				s << getKeyword(ID_END)->val;
			}
		}

		void unserialize(DataStream& s, Entity* entity)
		{
			while (!s.eof())
			{
				switch (parseID(s))
				{
				case ID_PROP:
					parseProp(s, entity);
					break;
				case ID_SKILL:
					parseSkill(s, entity);
					break;
				case ID_ERROR:
				default:
					JSB_EXCEPT("fail to parse entity id");
					break;
				}
			}
		}


	protected:



		ID parseID(DataStream& s)
		{
			int id;
			s >> id;

			for (size_t i = 0; ; ++i)
			{
				const Keyword* key = getKeyword(i);
				if (key == nullptr) break;
				if (key->val != id)	continue;
				return (ID)i;
			}

			return ID_ERROR;
		}

		template<class T>
		void parse(DataStream& s, T& val)
		{
			if (s.tell() + sizeof(T) > s.size())
				JSB_EXCEPT("fail to parse entity data");
			s >> val;
		}

		void parse(DataStream& s, String& val)
		{
			val.clear();
			s >> val;
			if (val.empty())
				JSB_EXCEPT("fail to parse entity data");
		}

		void parseEnd(DataStream& s)
		{
			if (parseID(s) != ID_END)
				JSB_EXCEPT("fail to parse entity data");
		}

		virtual void parseProp(DataStream& s, Entity* entity) = 0;
		//{
		//	Property property;
		//	parse(s, property.name);
		//	parse(s, property.hp);
		//	parse(s, property.str);
		//	parse(s, property.dex);
		//	parse(s, property.inte);
		//	parse(s, property.speed);
		//	parse(s, property.progress);

		//	parseEnd(s);

		//	entity->setProperty(property);

		//}

		virtual void parseSkill(DataStream& s, Entity* entity) = 0;
		//{
		//	String skill;
		//	parse(s, skill);
		//	parseEnd(s);

		//	SkillLuaConstructor cont;
		//	entity->getSkillContainer().addSkill(skill, cont);
		//}

		
	};
}

#endif