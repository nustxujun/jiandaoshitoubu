#include "Player.h"
#include "SkillInstance.h"
#include "PlayerSerializer.h"
#include "SkillTrigger.h"

using namespace JSB;

Player::Player(Identity id) :Entity(id)
{
	class ShellTrigger :public SkillTrigger
	{


	};


	mSkillCont.createTrigger<ShellTrigger>("shell_trigger");
}

void Player::serialize(DataStream& data)
{
	PlayerSerializer ps;

	ps.serialize(data, this);

}

void Player::unserialize(DataStream& data)
{
	PlayerSerializer s;
	s.unserialize(data, this);
}

Player* PlayerManager::createPlayer(Identity id)
{
	Player* player = new Player(id);
	auto ret = mPlayers.insert(std::make_pair(id, player));
	if (!ret.second)
	{
		delete player;
		JSB_EXCEPT("player id is existed.");
		return nullptr;
	}

	return player;
}

Player* PlayerManager::createMain(Identity id)
{
	Assert(mMain == nullptr);
	return mMain = createPlayer(id);
}

Player* PlayerManager::getPlayer(Identity id)
{
	auto ret = mPlayers.find(id);
	if (ret == mPlayers.end())
		return nullptr;
	return ret->second;
}

Player* PlayerManager::getMain()
{
	return mMain;
}


void PlayerManager::destroyPlayer(Identity id)
{
	auto ret = mPlayers.find(id);
	if (ret == mPlayers.end())
		return ;
	delete ret->second;
	mPlayers.erase(id);
}

