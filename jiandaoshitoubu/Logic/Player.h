#ifndef _Player_H_
#define _Player_H_

#include "Entity.h"
#include "Common.h"
#include "DataStream.h"

namespace JSB
{
	class Player: public Entity
	{
	public:
		Player(Identity id);
		void serialize(DataStream& data);
		void unserialize(DataStream& data);

		void useSkill(Environment& env){   }

	private:

	};

	class PlayerManager
	{
	public:
		Player* createPlayer(Identity id);
		Player* createMain(Identity id);
		Player* getPlayer(Identity id);
		Player* getMain();
		void destroyPlayer(Identity id);

	private:
		Player* mMain = 0;
		std::map<Identity, Player*> mPlayers;
	};
}

#endif