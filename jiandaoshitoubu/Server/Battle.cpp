#include "Battle.h"
#include "User.h"
#include "Monster.h"
#include "Environment.h"
#include "Events.h"
#include "CommonFile.h"
#include "SceneNode.h"
using namespace JSB;

Battle::Battle(SceneNode* node) : Scene(node), mMonster(node)
{
	
}

void Battle::init()
{
	using namespace luareg;

	CommonFile* cf = (CommonFile*)ResourceManager::getSingleton().getResource(STR("PlayerFunction.lua"));
	if (cf == nullptr)
		JSB_EXCEPT("cant find file PlayerFunction.lua");

	Executer::execute(mLuaState,
					  (const char*)cf->getData(),
					  cf->getSize(),
					  cf->getName().c_str());


	for (auto i : MonsterFactory::getSingleton().getTemplates())
	{
		mMonsterTemplates.push_back(i.first);
	}
}


void Battle::update(float elapsedTime)
{
	switch (mState)
	{
	case WAIT:
		if (getNode()->getUserCount() != 0)
			mState = BEGIN;
		break;
	case BEGIN:
		generatMonster();
		mState = FIGHT;
		break;
	case FIGHT:
		{
			action(elapsedTime);
		}
		break;
	case END:
		afterbattle();

		mState = BEGIN;
		break;
	default:
		break;
	}

}

void Battle::action(float elapsedTime)
{
	static float timer = 0;
	const float	t = 0.1f;

	timer += elapsedTime;
	if (timer < t) return;
	timer -= t;

	Environment env;
	env.addSide2(&mMonster);
	for (auto i : getNode()->getUsers())
		env.addSide1(i);
	size_t diecount = 0;
	for (auto i : getNode()->getUsers())
	{
		ServerEntity& p = *i;
		env.setVisitor(&p);
		Property& pp = p.getPropertyRef();
		pp.progress += pp.speed;
		
		if (pp.hp == 0)
			diecount++;
		if (pp.progress >= 100 && pp.hp > 0)
		{
			pp.progress = 0;
			p.useSkill(env);
		}

	}

	env.setVisitor(&mMonster);
	Property& pp = mMonster.getPropertyRef();
	pp.progress += pp.speed;
	if (pp.progress >= 100 && pp.hp > 0)
	{
		pp.progress = 0;
		mMonster.useSkill(env);
	}

	//env.setVisitor(Environment::SIDE1);

	//for (auto i : env.getChangedParteners())
	//{
	//	DataStream msg;
	//	msg << EVENT_S2C_PLAYER_INFO;
	//	msg << i->getID();
	//	i->serialize(msg);
	//	mUserMgr->broadcast(msg);

	//}

	//for (auto i : env.getChangedEnemies())
	//{
	//	DataStream msg;
	//	msg << EVENT_S2C_ENEMY_INFO;
	//	msg << i->getID();
	//	i->serialize(msg);
	//	mUserMgr->broadcast(msg);
	//}

	//for (auto i : env.output())
	//{
	//	DataStream msg;
	//	msg << EVENT_S2C_SERVER_PRINT;
	//	msg << i;
	//	mUserMgr->broadcast(msg);
	//}

	if (mMonster.getProperty().hp <= 0 || 
		getNode()->getUserCount() == 0 || 
		diecount == env.getPartenerCount())
		mState = END;

}

void Battle::generatMonster()
{
	//DataStream data;
	//Property prop;
	//prop.name = STR("monster");
	//prop.str = 0;
	//prop.dex = 0;
	//prop.hp = 100;
	//prop.inte = 0;
	//prop.progress = 0;
	//prop.speed = 10;
	//prop.level = 1;
	//prop.exp = 10;

	//mMonster.setProperty(prop);

	//SkillLuaConstructor cont;
	//mMonster.getSkillContainer().addSkill("attack", cont);

	int r = rand() % mMonsterTemplates.size();
	MonsterFactory::getSingleton().generate(mMonsterTemplates[r], mMonster);
}

void Battle::print(User* user, const String& str)
{
	DataStream msg;
	msg << EVENT_S2C_SERVER_PRINT;
	msg << str;
	user->send(msg);
}


void Battle::afterbattle()
{
	int exp = mMonster.getProperty().exp;
	String name = mMonster.getProperty().name;
	bool win = mMonster.getProperty().hp == 0;
	for (auto i : getNode()->getUsers())
	{
		Property& prop = i->getPropertyRef();
		if (win)
			prop.exp += exp;
		prop.hp = 100;
		prop.progress = 0;

		int level = callLua<int>("getCurrentLevel", prop.exp);
		if (level != prop.level)
		{
			prop.level = level;
			Stringstream ss;
			ss << STR("you get level ") << level;
			print(i, ss.str());
		}



		{
			DataStream msg;
			msg << EVENT_S2C_PLAYER_INFO;
			msg << i->getID();
			i->serialize(msg);
			i->send(msg);
		}
		if (win)
		{
		
			Stringstream ss;
			ss << STR("你击败了 ") << name <<
				STR(", 获得 ") << exp << STR(" 点经验");
			print(i,ss.str());

		}
		else
		{
			Stringstream ss;
			ss << STR("你被 ") << name << STR("打败了");
			print(i, ss.str());
		}
	}

}
