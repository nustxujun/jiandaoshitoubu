#ifndef _Entity_H_
#define _Entity_H_

#include "Common.h"
#include "Property.h"
#include "Buff.h"
#include "SkillContainer.h"
#include "DataStream.h"

namespace JSB
{

	class Entity
	{
	public:
		Entity(Identity id);

		Identity getID()const;

		//virtual void  action(Environment& cond);
		//virtual SkillResult hurt(const CastObject& sr);

		virtual void serialize(DataStream& data) = 0;
		virtual void unserialize(DataStream& data) = 0;
		
		const Property& getProperty()const;
		Property& getPropertyRef();

		void setProperty(const Property& prop);

		SkillContainer& getSkillContainer();
	protected:
		Property mProperty;
		SkillContainer mSkillCont;
		BuffContainer mBuffCont;

	private:
		Identity mID;

	};
}

#endif