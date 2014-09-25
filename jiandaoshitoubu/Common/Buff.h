#ifndef _Buff_H_
#define _Buff_H_

#include "Common.h"
#include "IteratorWrapper.h"
#include "Singleton.h"

namespace JSB
{
	class BuffResult
	{};

	class Buff
	{
	public:
		BuffResult affect(Entity* host );

	private:
	};

	class BuffContainer
	{
		typedef std::set<Identity> BuffSet;

	public:
		typedef IteratorWrapper<BuffSet> Buffs;

	public:
		void addBuff(Identity id);
		void removeBuff(Identity id);
		bool hasBuff(Identity id);
		Buffs getBuffs();
		BuffResult affect(Entity* host, Identity id);
	};
}

#endif