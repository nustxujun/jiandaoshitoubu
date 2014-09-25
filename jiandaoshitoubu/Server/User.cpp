#include "User.h"
#include "Socket.h"
#include "UserDataSerializer.h"
#include "Triggers.h"
#include "SkillEffect.h"
#include "Events.h"
#include "Environment.h"
#include "SceneNode.h"

using namespace JSB;

User::User(Identity id, SOCKET sock)
	:mSocket(sock), ServerEntity(id), mNode(0)
{
	Triggers::generatorTriggers(&mSkillCont);
}

Socket* User::getSocket()
{
	return &mSocket;
}

void User::send(DataStream& msg)
{
	size_t size = msg.size();
	mSocket.send(&size, sizeof(size));
	mSocket.send(msg.data(), msg.size());
}

void User::broadcast(DataStream& msg)
{
	if (mNode)
		mNode->broadcast(msg);
	else
		send(msg);
}

void User::serialize(DataStream& data)
{
	UserDataSerializer s;
	s.serialize(data, this);
}

void User::unserialize(DataStream& data)
{
	UserDataSerializer s;
	s.unserialize(data, this);
}

void User::useSkill(Environment& env)
{
	RandomTrigger* rt = (RandomTrigger*)mSkillCont.getTrigger(Triggers::RANDOM_TRIGGER);
	auto skill = rt->trigger(env);
	if (skill == nullptr) return;
	auto es = skill->trigger();
	for (auto& i : es)
	{
		for (auto &j : env.getTarget(i->getTarget()))
		{
			i->effect(env.getVisitor(), j);
		}
	}


	//Send msg


	DataStream msg;
	msg << EVENT_S2C_CAST;
	msg << getID();
	msg << skill->getName();
	msg << es.size();

	broadcast(msg);
}

void User::hurt(SkillEffectType type, int damage)
{
	mProperty.hp -= damage;
	max(mProperty.hp, 0);

	sendProperty(true);
}


void User::sendProperty(bool bc)
{
	DataStream msg;
	msg << EVENT_S2C_PLAYER_INFO;
	msg << getID();
	serialize(msg);
	if (bc)
		broadcast(msg);
	else
		send(msg);
}

SceneNode* User::getSceneNode()
{
	return mNode;
}

void User::setSceneNode(SceneNode* node)
{
	mNode = node;
}



User* UserManager::createUser(Identity id, SOCKET sock)
{
	User* user = new User(id, sock);

	auto ret = mUserMap.insert(std::make_pair(id, user));
	if (!ret.second)
	{
		delete user;
		JSB_EXCEPT("user id is existed.");
		return 0;
	}
	return user;
}

void UserManager::destroyUser(Identity id)
{
	auto ret = mUserMap.find(id);
	if (ret == mUserMap.end())
		return;
	delete ret->second;
	mUserMap.erase(ret);
}

User* UserManager::getUser(Identity id)
{
	auto ret = mUserMap.find(id);
	if (ret == mUserMap.end())
	{
		JSB_EXCEPT("user id is not existed.");
		return 0;
	}

	return ret->second;
}

size_t UserManager::getUserCount()const
{
	return mUserMap.size();
}

UserManager::Users UserManager::getUsers()
{
	return Users(mUserMap.begin(), mUserMap.end());
}

void UserManager::broadcast(DataStream& msg, User* without)
{
	for (auto i : mUserMap)
	{
		if (without == i.second)
			continue;
		i.second->send(msg);
	}
}
