#include "SkillInstance.h"


using namespace JSB;

SkillInstance::SkillInstance(SkillContainer* cont, const String& name) 
	:mContainer(cont), mName(name)
{

}

SkillInstance::~SkillInstance()
{
	
}

const String& SkillInstance::getName()const
{
	return mName;
}

