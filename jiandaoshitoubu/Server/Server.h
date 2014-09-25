#ifndef _Server_H_
#define _Server_H_

#include "Common.h"
#include "Socket.h"
#include "DataStream.h"
#include "User.h"
#include "EventManager.h"
#include "Battle.h"

namespace JSB
{
	class Server
	{
		struct Link
		{
			int nextSize = 0;
			Socket* sock = 0;
			User* user = 0;
		};

	public:
		Server();
		~Server();

		void start();
		void update();
		void stop();
		bool isStop()const;
	private:
		void loadMod();

		void processMsg();
		void checkNewLink();
		void createNewLink(Link& link, SOCKET id);
		void destroyLink(SOCKET id);
		

		void print(User* user, const String& str);
		void send(User* user, DataStream& msg);
		void broadcast(DataStream& msg, User* without = nullptr);

	private://event
		void registerEvent();

		struct UserParameter:public Parameter
		{
			User* user;
			DataStream msg;
		};

		void join(Parameter& msg);
		void leave(Parameter& msg);
		void clientInput(Parameter& msg);

		void skilllist(Parameter& msg);
		void selectskill(Parameter& msg);
	private:
		std::map<SOCKET, Link> mLinks;

		Socket mSocket;

		UserManager mUserMgr;

		std::map<String, Mod*> mMods;

		Battle mBattle;

		bool mStop = false;
	};
}

#endif