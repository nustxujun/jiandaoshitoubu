#include "Environment.h"
#include "ServerEntity.h"
#include "LogicType.h"

using namespace JSB;

void Environment::setVisitor(ServerEntity* ent)
{
	mVisitor = ent;

	if (mPartener.find(ent) != mPartener.end())
	{
	}
	else if (mEnemy.find(ent) != mEnemy.end())
	{
		mPartener.swap(mEnemy);
	}
	else
		JSB_EXCEPT("unknown visitor");
}

void Environment::addSide1(ServerEntity* ent)
{
	mPartener.insert(ent);
}

void Environment::addSide2(ServerEntity* ent)
{
	mEnemy.insert(ent);
}

size_t Environment::getPartenerCount()
{
	return mPartener.size();
}

size_t Environment::getEnemyCount()
{
	return mEnemy.size();
}


Environment::Entities Environment::getTarget(unsigned int target)
{
	Entities ents;

	auto check = [target](TargetType type)
	{
		return (target & type) != 0;
	};

	if (check(TT_SELF))
	{
		ents.insert(mVisitor);
	}

	if (check(TT_PARTNER) && !mPartener.empty())
	{

		if (check(TT_SINGLE))
		{
			ents.insert(*mPartener.begin());
		}
		else if(check(TT_TEAM))
		{
			ents.insert(mPartener.begin(), mPartener.end());
		}
	}
	
	if (check(TT_ENEMY) && !mEnemy.empty())
	{
		if (check(TT_SINGLE))
		{
			ents.insert(*mEnemy.begin());
		}
		else if (check(TT_TEAM))
		{
			ents.insert(mEnemy.begin(), mEnemy.end());
		}
	}



	return ents;
}
