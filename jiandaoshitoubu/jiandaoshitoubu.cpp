// jiandaoshitoubu.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "JSB.h"
#include<winsock.h>
#include <string>
#include <fstream>

using namespace JSB;

JSBGame* game;


int _tmain(int argc, _TCHAR* argv[])
{


	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(1, 1), &WSAData))
	{
		return 0;
	}

	game = new JSBGame;
	game->start();
	return 0;
}

