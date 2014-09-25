#ifndef _Skill_H_
#define _Skill_H_

#include "Common.h"
#include "IteratorWrapper.h"
#include "Singleton.h"



namespace JSB
{

	class Skill
	{
	public:
		Skill(const String& name, CommonFile* file);
		~Skill();
		
		CommonFile* getResFile();
		const String& getName();
	private:
		CommonFile* mRes;
		String mName;
	};



	class SkillManager : public Singleton < SkillManager >
	{
	public:
		typedef IteratorWrapper<std::map<String, Skill*>> Skills;

		Skills getSkills();
		Skill* createSkill(const String& name, CommonFile* file);
		Skill* getSkill(const String& name);
		size_t getSkillCount()const;
	private:
		std::map<String, Skill*> mSkills;

	};

}

#endif