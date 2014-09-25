#include "JSB.h"
#include "Server.h"
#include "Client.h"
#include "EventManager.h"
#include "Resource.h"
#include "FindFile.h"

#include "CommonFile.h"
#include "Profile.h"

using namespace JSB;

JSBGame::JSBGame()
{
	initialize();
}

JSBGame::~JSBGame()
{

	uninitialize();

}


void JSBGame::start( )
{

	Client client;

	while (true)
	{
		if (mServer)
			mServer->update();
		client.update();
	}
}

void JSBGame::initialize()
{
	new EventManager;
	new ResourceManager;

	regResFac();
	loadData();

	EventManager::getSingleton().registerEvent(EVENT_C_START_SERVER, this, &JSBGame::startServer);
}

void JSBGame::loadData()
{

}


void JSBGame::uninitialize()
{
	delete mServer;

	delete EventManager::getSingletonPtr();
	delete ResourceManager::getSingletonPtr();
}

void JSBGame::regResFac()
{
	ResourceManager& resmgr = ResourceManager::getSingleton();

	static CommonFileFactory commontextfac;
	resmgr.registerFactory(CommonFile::RESOURCE_TYPE, &commontextfac);

	static ProfileFactory profilefac;
	resmgr.registerFactory(Profile::RESOURCE_TYPE, &profilefac);
}

void JSBGame::unregResFac()
{}

void JSBGame::serverrunning()
{
	try
	{
		Server server;
		server.start();
		while (!server.isStop())
		{
			server.update();
			Sleep(1);
		}
	}
	catch (Exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void JSBGame::startServer(Parameter& para)
{
	mServer = new Server;
	mServer->start();
}

