#ifndef _EventManager_H_
#define _EventManager_H_

#include "Common.h"
#include "Singleton.h"
#include "Events.h"

namespace JSB
{

	class Parameter
	{
	public:
	};

	class EventManager :public Singleton<EventManager>
	{
		class EventObj
		{
		public :
			virtual void operator()(Parameter& p) = 0;
		};

		struct NormalObj :public EventObj
		{
			void(*func)(Parameter&);
			void operator()(Parameter& msg)
			{
				(*func)(msg);
			}
		};

		template<class T>
		struct MemberObj : public EventObj
		{
			T* inst;
			void(T::*func)(Parameter&);
			void operator()(Parameter& msg)
			{
				(inst->*func)(msg);
			}
		};
	public :
		void registerEvent(Events eve, void(*func)(Parameter& msg))
		{
			NormalObj* obj = new NormalObj;
			obj->func = func;

			mEvents[eve].push_back(obj);
		}
			

		template<class T>
		void registerEvent(Events eve, T* inst, void(T::*func)(Parameter& msg))
		{
			MemberObj<T>* obj = new MemberObj<T>;
			obj->func = func;
			obj->inst = inst;
			mEvents[eve].push_back(obj);
		}

		void notifyEvent(Events eve, Parameter& msg)
		{
			Assert(eve < EVENT_NUM);
			for (auto i : mEvents[eve])
			{
				(*i)(msg);
			}
		}

	private:
		typedef std::vector<EventObj*> EventList;

		EventList mEvents[EVENT_NUM];
	};

}

#endif