#ifndef _Monster_H_
#define _Monster_H_

#include "ServerEntity.h"
#include "IteratorWrapper.h"
namespace JSB
{
	class Monster : public ServerEntity
	{
	public:
		Monster();

		void serialize(DataStream& data);
		void unserialize(DataStream& data);

		void useSkill(Environment& env);
		void hurt(SkillEffectType type, int damage) ;

	};



	class MonsterFactory:public Singleton<MonsterFactory>
	{
	public :
		typedef std::map<String, Monster*> Templates;
		typedef IteratorWrapper<Templates> TemplateIterator;
	public:
		void add(CommonFile* file);
		void generate(const String& name, Monster& m);
		TemplateIterator getTemplates();
		
	private:
		std::map<String, Monster*> mTemplates;
	};
}

#endif