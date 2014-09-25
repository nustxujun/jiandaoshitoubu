#ifndef _SkillContainer_H_
#define _SkillContainer_H_

#include "Common.h"
#include "IteratorWrapper.h"
#include "SkillInstance.h"
namespace JSB
{
	class SkillContainer
	{
		typedef std::map<String, SkillInstance*> SkillMap;
		typedef std::vector<SkillInstance*> SkillVec;
		typedef std::map<String, SkillTrigger*> TriggerMap;
	public:
		typedef IteratorWrapper<SkillMap> Skills;

	public:
		SkillContainer(Entity* caster);
		~SkillContainer();

		void addSkill(const String& name, const SkillInstanceConstructor & cont);
		void removeSkill(const String& name);
		bool hasSkill(const String& name);
		Skills getSkills();
		size_t getSkillCount()const;
		void clear();

		template<class T>
		void createTrigger(const String& name)
		{
			SkillTrigger* st = new T();

			auto ret = mTriggers.insert(std::make_pair(name, st));
			if (!ret.second)
			{
				JSB_EXCEPT("same trigger name is existed");
				delete st;
			}
		}

		SkillTrigger* getTrigger(const String& name);
		Entity* getCaster();

	private:
		SkillMap mSkills;
		Entity* mCaster;
		TriggerMap mTriggers;
	};
}

#endif