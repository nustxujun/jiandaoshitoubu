#ifndef _Client_H_
#define _Client_H_

#include "Common.h"
#include "DataStream.h"
#include "EventManager.h"
#include "Player.h"
#include "LogicHUD.h"
namespace JSB
{
	
	class Client
	{
		enum UIType
		{
			COMMON_O,
			COMMON_I,
			COMMAND,
			VIEW,
			NUM
		};
	public:
		Client();
		~Client();


		void login();

		void update();

		void send(DataStream& msg);

	private:
		void load();
		HUD& ui(UIType t);
		void setFocus(UIType t);

		void processMsg();

		void parseCommand(const String&);
		enum MenuType
		{
			MAIN_MENU,
			SKILL,
		};

		void setMenu(MenuType menu);

		enum ViewType
		{
			PROPERTY,
		};
		void updateView(ViewType t, bool force = true);
	private:
		void registerEvent();

		void loginComplete(DataStream& msg);
		void updatePlayer(DataStream& msg);
		void removePlayer(DataStream& msg);

		void skilllist(DataStream& para);
		void processCasting(DataStream& para);

		void serverPrint(DataStream& msg);
		void clientInput(DataStream& msg);
 
	private:
		Socket* mSocket = 0;

		PlayerManager mPlayerMgr;
		Console mConsole;

		HUD* mHUD[NUM];
		Profile* mProfile = 0;

	};
}

#endif