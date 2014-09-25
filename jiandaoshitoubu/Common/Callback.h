#ifndef _Callback_H_
#define _Callback_H_

namespace JSB
{
	template<class ... P>
	class Callback
	{
	public:
		virtual void operator()(P ... paras) = 0;
	};


	template<class F, class ... P>
	class NormalFunction: public Callback< P...>
	{
	public:
		typedef F Function;
		Function func;

		NormalFunction(Function f) : func(f)
		{
		}

		void operator()(P... paras) 
		{
			 func(paras...);
		}
	};

	template<class O,class F, class ... P>
	class MemeberFunction : public Callback < P... >
	{
	public:
		typedef F Function;
		O* obj;
		Function func;

		MemeberFunction(O* o, Function f) : obj(o), func(f)
		{}

		void operator()(P...paras)
		{
			(obj->*func)(paras);
		}
	};



}

#endif