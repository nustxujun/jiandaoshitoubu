#ifndef _Environment_H_
#define _Environment_H_

#include "Singleton.h"
#include "Common.h"
#include "LogicType.h"
#include "Property.h"
#include "IteratorWrapper.h"
namespace JSB
{


	class Environment
	{
	public:
		typedef std::set<ServerEntity*> EntitySet;
		typedef std::set<ServerEntity*> Entities;
	public:


		void setVisitor(ServerEntity* ent);
		void addSide1(ServerEntity* ent);
		void addSide2(ServerEntity* ent);

		size_t getPartenerCount();
		size_t getEnemyCount();

		Entities getTarget(unsigned int target);
		ServerEntity* getVisitor() { return mVisitor; }
	private:
		enum Side
		{
			SIDE1,
			SIDE2,
			NUM_SIDE
		};
		EntitySet mPartener;
		EntitySet mEnemy;
		ServerEntity* mVisitor;

	};

}

#endif