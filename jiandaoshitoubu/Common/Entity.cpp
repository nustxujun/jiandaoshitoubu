#include "Entity.h"

using namespace JSB;

Entity::Entity(Identity id) :mSkillCont(this), mID(id)
{
	
}


Identity Entity::getID()const
{
	return mID;
}

const Property& Entity::getProperty()const
{
	return mProperty;
}

Property& Entity::getPropertyRef()
{
	return mProperty;
}

void Entity::setProperty(const Property& prop)
{
	mProperty = prop;
}

SkillContainer& Entity::getSkillContainer()
{
	return mSkillCont;
}

