#include "Server.h"
#include "EventManager.h"
#include "Resource.h"
#include "FindFile.h"
#include "Mod.h"
#include "Skill.h"
#include "CommonFile.h"
#include "SkillLua.h"
#include "Monster.h"

using namespace JSB;

Server::Server():
	mSocket(Socket::DEFAULT_PORT),
	mBattle(&mUserMgr)
{
	registerEvent();

	new SkillManager;
	new MonsterFactory;

	loadMod();
	mBattle.init();
}

Server::~Server()
{
	for (auto i : mLinks)
		delete i.second.sock;
	mLinks.clear();

	delete SkillManager::getSingletonPtr();
}

void Server::start()
{
	if (mSocket.getState() != SS_OK)
		JSB_EXCEPT("server socket error");
	mStop = false;
}

void Server::stop()
{
	mStop = true;
}

bool Server::isStop()const
{
	return mStop;
}

void Server::processMsg()
{
	checkNewLink();

	EventManager& evemgr = EventManager::getSingleton();
	std::vector<SOCKET> discon;

	UserParameter up;
	auto endi = mLinks.end();
	for (auto i = mLinks.begin(); i != endi; )
	{
		Link& link = i->second;

		link.sock->update();


		if (link.sock->getState() != SS_OK)
		{
			discon.push_back(i->first);
			++i;
			continue;
		}


		if (link.nextSize == 0)
		{
			if (!link.sock->recv(&link.nextSize, sizeof(link.nextSize)))
				++i;
		}
		else
		{
			up.msg.reserve(link.nextSize);
			if (link.sock->recv((void*)up.msg.data(), link.nextSize))
			{
				up.msg.skipW(link.nextSize);
				link.nextSize = 0;
				//proc msg
				Events eve;
				up.msg >> eve;
				up.user = link.user;
				evemgr.notifyEvent(eve, up);
			}
			else
				++i;
		}

	}

	for (auto i : discon)
		destroyLink(i);
}

void Server::update()
{
	if (mStop) return;
	processMsg();

	static int timer = GetTickCount();
	float elapsed = (GetTickCount() - timer) / 1000.f;
	timer = GetTickCount();
	mBattle.update(elapsed);
}

void Server::createNewLink(Link& link, SOCKET id)
{
	link.user = mUserMgr.createUser(id, id);
	link.sock = link.user->getSocket();
}

void Server::destroyLink(SOCKET id)
{
	auto ret = mLinks.find(id);
	if (ret == mLinks.end()) return;
	mUserMgr.destroyUser(ret->second.user->getID());
	mLinks.erase(ret);

}

void Server::checkNewLink()
{

	SOCKET id = mSocket.accept();
	if (INVALID_SOCKET == id)
		return;

	createNewLink(mLinks[id], id);
}

void Server::print(User* user, const String& str)
{
	DataStream msg;
	msg << EVENT_S2C_SERVER_PRINT << str; 
	send(user, msg);
}

void Server::send(User* user, DataStream& msg)
{
	user->send(msg);
	//Socket* sock = mLinks[user->getID()].sock;
	//size_t size = msg.size();
	//sock->send(&size, sizeof(size));
	//sock->send(msg.data(), msg.size());
}

void Server::broadcast(DataStream& msg, User* without)
{
	mUserMgr.broadcast(msg, without);
}

void Server::loadMod()
{
	FindFile ff;
	FindFile::FileList fl = ff.find(STR("Data\\Mod\\*.*"), FT_DIR, 0);

	for (auto i : fl)
	{
		mMods[i.name] = new Mod(i.name, i.full);
	}

}



void Server::registerEvent()
{
	EventManager& evemgr = EventManager::getSingleton();

	evemgr.registerEvent(EVENT_C2S_LOGIN, this, &Server::join);
	evemgr.registerEvent(EVENT_C2S_CLIENT_INPUT, this, &Server::clientInput);
	evemgr.registerEvent(EVENT_C2S_SKILL_LIST, this, &Server::skilllist);
	evemgr.registerEvent(EVENT_C2S_CHOSE_SKILL, this, &Server::selectskill);


}

void Server::join(Parameter& para)
{
	UserParameter& up = (UserParameter&)para;

	bool newplayer;
	up.msg >> newplayer;

	if (newplayer)
	{ 
		String name;
		up.msg >> name;
		
		Resource* res = ResourceManager::getSingleton().getResource(STR("newcharacter.ent"));
		CommonFile* f = (CommonFile*)res;
		DataStream data;
		data.write(f->getData(), f->getSize());
		up.user->unserialize(data);

		Property& prop = up.user->getPropertyRef();
		prop.name = name;
	}
	else
	{
		up.user->unserialize(up.msg);
	}

	//for (auto i : mLinks)
	{
		DataStream msg;
		User* user = up.user;
		msg << EVENT_S2C_LOGIN_COMPLETE;
		msg << user->getID();

		user->serialize(msg);
		send(up.user, msg);
	}

	{
		DataStream msg;
		msg << EVENT_S2C_PLAYER_INFO;
		msg << up.user->getID();

		up.user->serialize(msg);
		broadcast(msg, up.user);
	}

}

void Server::clientInput(Parameter& para)
{
	UserParameter& up = (UserParameter&)para;
}


void Server::skilllist(Parameter& para)
{
	UserParameter& up = (UserParameter&)para;
	SkillManager& sm = SkillManager::getSingleton();


	DataStream msg;
	msg << EVENT_S2C_SKILL_LIST;
	//msg << sm.getSkillCount();

	Stringstream ss;
	auto skills = sm.getSkills();
	size_t index = 1;
	for (auto i : skills)
	{
		ss << index++ << STR(".");
		ss << i.second->getName();
		ss << STR(" ");
	}

	msg << ss.str();
	up.user->send(msg);
}

void Server::selectskill(Parameter& para)
{
	UserParameter& up = (UserParameter&)para;
	SkillContainer& sc = up.user->getSkillContainer();
	sc.clear();

	SkillManager& sm = SkillManager::getSingleton();
	std::vector<int> nskills;
	nskills.resize(sm.getSkillCount());

	String skills;
	up.msg >> skills; 

	Stringstream ss;
	ss << skills;
	size_t count = skills.size();

	if (count > sm.getSkillCount())
	{
		print(up.user, STR("invalied skill count"));
		return;
	}

	for (size_t i = 0; i < count; ++i)
	{
		Char c;
		ss >> c;
		Stringstream cv;
		cv << c;
		int index = 0;
		cv >> index ;
		if (index == 0)
		{
			print(up.user, STR("invalied skill id "));
			return;
		}
		else if (nskills[index - 1] != 0)
		{
			print(up.user, STR("same skill id "));
			return;
		}
		nskills[index - 1] = i + 1;
	}
	size_t index = 0;
	SkillLuaConstructor cont;
	for (auto i : sm.getSkills())
	{
		if (nskills[index] == 0)
		{
			++index;
			continue;
		}
		sc.addSkill(i.first, cont);
		++index;

	}



}