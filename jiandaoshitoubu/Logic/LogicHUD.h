#ifndef _LogicHUD_H_
#define _LogicHUD_H_

#include "HUD.h"
#include "EventManager.h"
#include "Callback.h"
namespace JSB
{
	class CommonOutput:public HUD
	{
	public:
		CommonOutput(Console* con);


		void write(const String& str);
		HUD& operator << (const Operator& val);
	public:
		bool mEndl = false;
	};


	class CommonInput : public HUD
	{
	public:
		CommonInput(Console* con);
		~CommonInput();

		void write(const String& str);
		String read();
		void clear();
		void setFocus(bool v);

		void processInput(const String& str);
	};


	class Menu: public HUD
	{
	public:
		Menu(Console* con);
		~Menu();

		void setFocus(bool v);
		void processKeyEvent(const KEY_EVENT_RECORD& r);

		void processCommand(Parameter& para);

		typedef Callback<const String&> Listener;
		typedef std::vector<String> Menulist;
		
		template<class T>
		void setMenu( T t)
		{
			clearMenu();
			mListener = new NormalFunction<T, const String&>(t);
		}

		void clearMenu()
		{
			if (mListener == nullptr) return;
			delete mListener;
			mListener = nullptr;
		}

	private:
		Listener* mListener = nullptr;
	};


	class CommonView :public HUD
	{
	public:
		CommonView(Console* c);
		~CommonView();


	};
}

#endif