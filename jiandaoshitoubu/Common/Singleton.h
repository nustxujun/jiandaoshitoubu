#ifndef _Singleton_H_
#define _Singleton_H_

#include "Common.h"

namespace JSB
{
	template <class T>
	class Singleton
	{
	public:
		Singleton()
		{
			T*& inst = getInstance();
			Assert(inst == nullptr);
			inst = (T*)this;
		}

		~Singleton()
		{
			getInstance() = nullptr;
		}

		static T& getSingleton()
		{
			return *getInstance();
		}

		static T* getSingletonPtr()
		{
			return getInstance();
		}
	private:

		static T*& getInstance()
		{
			static T* inst = nullptr;
			return inst;
		}


	};
}


#endif