#ifndef _HUD_H_
#define _HUD_H_

#include "Console.h"
#include <list>
namespace JSB
{
	class LogicHUD;
	class HUD
	{

		struct Window
		{
			String str;
			std::vector<WORD> attrs;
			WORD lastattr;
			RECT rect;
			bool endl;
		};
	public:

		enum Operator
		{
			ENDL,
		};

		enum Attribute
		{
			DEFAULT,
			TEXT_RED,
			TEXT_GREEN,
			TEXT_BLUE ,
			TEXT_YELLOW,
			TEXT_PINK,
			TEXT_CYAN,
			TEXT_LIGHT,
			TEXT_DARK,

			BG_BLUE,
			BG_GREEN,
			BG_RED,
			BG_YELLOW,
			BG_PINK,
			BG_CYAN,
			BG_LIGHT,

		};

		HUD(Console* con);
		virtual ~HUD();

		template<class T>
		HUD& operator<< (const T& val)
		{
			Stringstream ss;
			ss << val;
			write(ss.str());
			return *this;
		}

		virtual HUD& operator << (const Operator& val);
		virtual HUD& operator << (const Attribute& val);

		template<class T>
		HUD& operator >> (T& val)
		{
			String r = read();
			Stringstream ss;
			ss << r;
			ss >> val;
			return *this;
		}

		virtual void write(const String& str);
		virtual String read();
		virtual void clear();

		virtual void setFocus(bool val);
		
		void setRect( const RECT& rect);
		const RECT& getRect();

	protected:
		void drawBox(const RECT& rect);
		void convertAttribute(WORD& att, Attribute a);

	protected:
		Console* mConsole;
		RECT mRect;
		COORD mCurpos;
		WORD mLastAttr;
		bool mFocus;
	};
}

#endif