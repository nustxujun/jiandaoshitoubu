#ifndef _User_H_
#define _User_H_

#include "Common.h"
#include "DataStream.h"
#include "Socket.h"
#include "IteratorWrapper.h"
#include "ServerEntity.h"

namespace JSB
{
	class User :public ServerEntity
	{
		friend class UserManager;
	public:
		User(Identity id, SOCKET sock);
		void serialize(DataStream& data);
		void unserialize(DataStream& data);

		Socket* getSocket();

		SceneNode* getSceneNode();
		void setSceneNode(SceneNode* node);

		void send(DataStream& msg);
		void broadcast(DataStream& msg);

		void useSkill(Environment& env) ;
		void hurt(SkillEffectType type, int damage);

	public://msg
		void sendProperty(bool broadcast = false);
		

	private:
		Socket mSocket;
		SceneNode* mNode;
	};


	class UserManager
	{
	public :
		typedef std::map<Identity, User*> UserMap;
		typedef IteratorWrapper<UserMap> Users;
	public:
		User* createUser(Identity id, SOCKET sock);
		void destroyUser(Identity id);

		User* getUser(Identity id);

		size_t getUserCount()const;

		Users getUsers();

		void broadcast(DataStream& msg, User* without = nullptr);

	private:
		UserMap mUserMap;
	};
}

#endif