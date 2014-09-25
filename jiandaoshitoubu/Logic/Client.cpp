#include "Client.h"
#include "Socket.h"
#include "Events.h"
#include "EventManager.h"
#include "Resource.h"
#include "Profile.h"
#include "CommonFile.h"
#include <Windows.h>

using namespace JSB;
using namespace std;

PlayerManager* pm = 0;
BOOL WINAPI consoleProc(DWORD e)
{
	switch (e)
	{
	case CTRL_CLOSE_EVENT:
	{

		//Player* p = pm->getMain();
		//if (p == nullptr)  break;
		//DataStream data;
		//p->serialize(data);

		//ofstream o("data\\save\\save.sav", ios::binary);
		//o << "SAVE";
		//o.write(data.data(), data.size());
		break;
	}
	default:
		break;
	}
	return TRUE;
}


Client::Client() 
{
	load();
}

Client::~Client()
{

}


void Client::load()
{
	pm = &mPlayerMgr;
	registerEvent();

	::SetConsoleCtrlHandler(consoleProc, TRUE);


	mHUD[COMMON_O] = new CommonOutput(&mConsole);
	mHUD[COMMON_I] = new CommonInput(&mConsole);
	mHUD[COMMAND] = new Menu(&mConsole);
	mHUD[VIEW] = new CommonView(&mConsole);

	//*mHUD.setInputListener(lambda);



	String saving = STR("data\\save\\save.sav");
	ResourceManager& resmgr = ResourceManager::getSingleton();
	CustomParams para;
	para[STR("path")] = saving;
	mProfile = (Profile*)resmgr.createResource(STR("save"), Profile::RESOURCE_TYPE, &para);


}

HUD& Client::ui(UIType t)
{
	return *mHUD[t];
}

void Client::setFocus(UIType t)
{
	static UIType last = t;
	if (t != last)
		mHUD[last]->setFocus(false);
	last = t;
	mHUD[t]->setFocus(true);
}


void Client::update()
{
	mConsole.update();

	enum
	{
		MENU,
		GAME,
	};

	static int state = MENU;

	switch (state)
	{
	case MENU:
	{
		ui(COMMAND).clear();
		ui(COMMAND) << HUD::DEFAULT << "1.Singleplayer" << HUD::ENDL << "2.Multiplayer" << HUD::ENDL;
		
		int select = 0;
		ui(COMMON_I) >> select;
		ui(COMMAND).clear();
		String addr;
		switch (select)
		{
		case 1:
			EventManager::getSingleton().notifyEvent(EVENT_C_START_SERVER, Parameter());
			addr = STR("127.0.0.1");
			break;
		case 2:
		{
			ifstream f("config.ini");
			if (!f)
			{
				ui(COMMAND) << STR("host ip:");
				ui(COMMON_I) >> addr;
			}
			else
				f >> addr;
		}
			break;
		default:
			ui(COMMON_O) << HUD::DEFAULT << "error index" << HUD::ENDL;

			break;
		}

		if (addr.empty())
			break;

		ui(COMMON_O) << HUD::DEFAULT << "connecting...";
		mSocket = new Socket(addr);

		if (mSocket->getState() != SS_OK)
		{
			ui(COMMON_O) << HUD::TEXT_RED << "failed" << HUD::DEFAULT << HUD::ENDL;
			ui(COMMON_O) << "socket error state:" << mSocket->getState() << HUD::ENDL;
		}
		else
		{
			ui(COMMON_O) << HUD::TEXT_GREEN << "ok" << HUD::DEFAULT << HUD::ENDL;

			setFocus(COMMON_I);

			state = GAME;
			login();

			setMenu(MAIN_MENU);
		}

	}
		break;
	case GAME:
	{
		if (mSocket->getState() != SS_OK)
		{
			ui(COMMON_O) << HUD::TEXT_RED << "lost server" << HUD::ENDL;
			delete mSocket;
			mSocket = 0;
			state = MENU;
		}
		else
		{
			processMsg();
		}
	}
		break;

	}

}



void Client::send(DataStream& msg)
{
	Assert(mSocket);

	size_t size = msg.size();
	mSocket->send(&size, sizeof(size));
	mSocket->send(msg.data(), size);
}

void Client::processMsg()
{
	static int nextSize = 0;

	struct Param :public Parameter
	{
		DataStream msg;
	};


	Param p;
	while (true)
	{
		mSocket->update();
		if (nextSize == 0)
		{
			if (!mSocket->recv(&nextSize, sizeof(nextSize)))
				return;
		}
		else
		{
			p.msg.clear();
			p.msg.reserve(nextSize);
			if (mSocket->recv((void*)p.msg.data(), nextSize))
			{
				p.msg.skipW(nextSize);

				//proc msg
				Events eve;
				p.msg >> eve;


				EventManager::getSingleton().notifyEvent(eve, p);

				nextSize = 0;

			}
			else 
				return;
		}
	}

}


void Client::login()
{

	DataStream msg;
	msg << EVENT_C2S_LOGIN;

	if (mProfile && mProfile->getState() == RS_OK)
	{
		msg << false;
		msg.write(mProfile->getData(), mProfile->getSize());
	}
	else
	{
		msg << true;//newplayer
		String name;
		ui(COMMAND) << "Name: ";
		ui(COMMON_I) >> name;
		msg << name;

		ui(COMMAND).clear();
	}

	send(msg);
}


void Client::registerEvent()
{
	EventManager& evemgr = EventManager::getSingleton();

	typedef void(Client::*Function)(Parameter&);

	evemgr.registerEvent(EVENT_S2C_LOGIN_COMPLETE, this, (Function)&Client::loginComplete);
	evemgr.registerEvent(EVENT_S2C_PLAYER_INFO, this, (Function)&Client::updatePlayer);
	evemgr.registerEvent(EVENT_S2C_REMOVE_PLAYER, this, (Function)&Client::removePlayer);
	evemgr.registerEvent(EVENT_S2C_SERVER_PRINT, this, (Function)&Client::serverPrint);
	evemgr.registerEvent(EVENT_S2C_REQEST_INPUT, this, (Function)&Client::clientInput);
	evemgr.registerEvent(EVENT_S2C_SKILL_LIST, this, (Function)&Client::skilllist);
	evemgr.registerEvent(EVENT_S2C_CAST, this, (Function)&Client::processCasting);

	
}


void Client::loginComplete(DataStream& msg)
{
	Identity id;
	msg >> id;

	Player* ply = mPlayerMgr.createMain(id);

	ply->unserialize(msg);

	ui(COMMON_O) << "login complete." << HUD::ENDL;

	updateView(PROPERTY);

}

void Client::updatePlayer(DataStream& msg)
{
	Identity id;
	msg >> id;

	Player* ply = mPlayerMgr.getPlayer(id);
	if (ply == nullptr)
	{
		ply = mPlayerMgr.createPlayer(id);
		ui(COMMON_O) << "new player enter" << HUD::ENDL;
	}

	ply->unserialize(msg);

	if (ply == mPlayerMgr.getMain())
	{
		DataStream data;
		ply->serialize(data);
		mProfile->save(data.data(), data.size());
		//ui(COMMON_O) << HUD::TEXT_CYAN << "checkpoint" << HUD::ENDL;
		
		updateView(PROPERTY, false);
	}

}


void Client::skilllist(DataStream& para)
{
	ui(COMMAND).clear();

	String str; 
	para >> str;
	ui(COMMAND) << HUD::TEXT_LIGHT << str << HUD::ENDL;

	ui(COMMAND) << HUD::DEFAULT << STR("input skill order (example: 12231):");


	auto lambda = [this](const String& cmd)
	{
		DataStream msg;
		msg << EVENT_C2S_CHOSE_SKILL;
		msg << cmd;
		send(msg);
		setMenu(MAIN_MENU);
	};

	((Menu*)mHUD[COMMAND])->setMenu(lambda);

}

void Client::processCasting(DataStream& para)
{
	Identity caster;
	para >> caster;
	String skillname;
	para >> skillname;

	ui(COMMON_O) << caster << STR(" cast ") << skillname << HUD::ENDL;


}




void Client::removePlayer(DataStream& msg)
{
	Identity id;
	msg >> id;
	mPlayerMgr.destroyPlayer(id);
	
}

void Client::serverPrint(DataStream& msg)
{
	String str;
	msg >> str;
	ui(COMMON_O) << HUD::DEFAULT <<  str << HUD::ENDL;

}

void Client::clientInput(DataStream& msg)
{
	String in;
	ui(COMMON_I) >> in;
	DataStream input;
	input << EVENT_C2S_CLIENT_INPUT;
	input << in;
	send(input);
}

void Client::parseCommand(const String& cmd)
{

}


void Client::setMenu(MenuType menu)
{
	ui(COMMAND).clear();
	((Menu*)mHUD[COMMAND])->clearMenu();
	switch (menu)
	{
	case MAIN_MENU:
		{
			auto lambda = [this](const String& cmd)
			{
				if (cmd == STR("1"))
				{
					DataStream msg;
					msg << EVENT_C2S_SKILL_LIST;
					send(msg);
				}
				else if (cmd == STR("2"))
				{
					setMenu(SKILL);
				}
			};

			ui(COMMAND) << STR("1.select skill") << HUD::ENDL;
			ui(COMMAND) << STR("2.check skill") << HUD::ENDL;


			((Menu*)mHUD[COMMAND])->setMenu(lambda);
		}
		break;
	case SKILL:
		{
			Player* p = mPlayerMgr.getMain();
			
			size_t index = 0;
			for (auto i : p->getSkillContainer().getSkills())
			{
				index++;
				Stringstream ss;
				ss << index << STR(".") << i.first;

				ui(COMMAND)<< ss.str() << HUD::ENDL;
			}

			auto lambda = [this](const String& cmd)
			{
				setMenu(MAIN_MENU);
			};

			((Menu*)mHUD[COMMAND])->setMenu(lambda);


		}
		break;


	}


}

void Client::updateView(ViewType t, bool force)
{
	static ViewType lastType = t;
	if (t != lastType && !force) return;
	lastType = t;
	ui(VIEW).clear();
	switch (t)
	{
	case PROPERTY:
		{
			const Property& prop = mPlayerMgr.getMain()->getProperty();
			ui(VIEW) << HUD::DEFAULT << STR(" 血:");
			size_t cont = prop.hp / 10;
			String hp;
			for (size_t i = 0; i < cont; ++i)
				hp.push_back(' ');
			ui(VIEW) << HUD::BG_RED  << hp << HUD::ENDL;


			ui(VIEW) << HUD::DEFAULT << STR(" 力:") << prop.str << HUD::ENDL;
			ui(VIEW) << HUD::DEFAULT << STR(" 敏:") << prop.dex << HUD::ENDL;
			ui(VIEW) << HUD::DEFAULT << STR(" 智:") << prop.inte << HUD::ENDL;
			ui(VIEW) << HUD::DEFAULT << STR(" 速:") << prop.speed << HUD::ENDL;
			ui(VIEW) << HUD::DEFAULT << STR(" 级:") << prop.level << HUD::ENDL;
			ui(VIEW) << HUD::DEFAULT << STR(" 经验:") << prop.exp << HUD::ENDL;

		}
		break;
	}
}
