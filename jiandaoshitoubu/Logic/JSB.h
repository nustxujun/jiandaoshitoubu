#ifndef _JSB_H_
#define _JSB_H_

#include <thread>
#include "EventManager.h"
#include "Server.h"
namespace JSB
{
	class JSBGame
	{
	public:
		JSBGame();
		~JSBGame();

		void start();
	private:
		void initialize();
		void uninitialize();

		void regResFac();
		void unregResFac();

		void loadData();

		void startServer(Parameter& para);
		static void serverrunning();


		Server* mServer = 0;
	};

}

#endif