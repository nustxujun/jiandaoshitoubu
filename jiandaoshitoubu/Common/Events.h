#ifndef _Events_H_
#define _Events_H_

namespace JSB
{
	enum Events
	{
		//local
		EVENT_C_START_SERVER,

		EVENT_C_COMMON_INPUT,


		//remote
		EVENT_C2S_LOGIN,
		EVENT_S2C_LOGIN_COMPLETE,
		EVENT_S2C_PLAYER_INFO,
		EVENT_S2C_REMOVE_PLAYER,

		EVENT_S2C_CAST,

		EVENT_S2C_ENEMY_INFO,

		EVENT_C2S_SKILL_LIST,
		EVENT_S2C_SKILL_LIST,
		EVENT_C2S_CHOSE_SKILL,



		EVENT_S2C_SERVER_PRINT,
		EVENT_C2S_CLIENT_INPUT,
		EVENT_S2C_REQEST_INPUT,

		EVENT_NUM
	};

}

#endif