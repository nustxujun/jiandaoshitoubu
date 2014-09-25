#ifndef _Console_H_
#define _Console_H_

#include "Common.h"
#include <Windows.h>
namespace JSB
{
	class Console
	{
		class Callback
		{
		public:
			virtual void operator()(const Char& record)
			{
			};
		};

		template<class T>
		class BlockingReadFunctor :public Callback
		{
		public:
			T func;
			BlockingReadFunctor(T f) :func(f) {}
			void operator()(const Char& record)	{ func(record);}
		};

		class KeyEventListener
		{
		public:
			virtual void operator()(const KEY_EVENT_RECORD& record) = 0;
		};

		template<class F>
		class NormalFunction :public KeyEventListener
		{
		public:
			F func;
			NormalFunction(F f) :func(f){}
			void operator()(const KEY_EVENT_RECORD& record)
			{
				return func(record);
			}

		};

		template<class O, class F>
		class ClassMemberFunction :public KeyEventListener
		{
		public:
			F func;
			O* obj;

			ClassMemberFunction(O* o, F f) :obj(o), func(f){}

			void operator()(const KEY_EVENT_RECORD& record)
			{
				return (obj->*func)(record);
			}

		};

	public:
		void write(const Char* buffer, size_t size);
		void write(size_t x, size_t y, const Char* buffer, size_t size);
		void write(const String& str);
		void write(size_t x, size_t y, const String& str);
		void fill(size_t x, size_t y, Char c, size_t len);

		
		template<class T>
		String read(T callback)
		{
			return read((Callback*)&BlockingReadFunctor<T>(callback));
		}

		String read(Callback* callback = &Callback());
		String getLine(size_t x, size_t y, size_t length);

		Char getChar();
		bool getKeyEvent(KEY_EVENT_RECORD& record);

		template<class F>
		void setKeyListener(const String& name, F func)
		{
			auto ret = mKeyListeners.insert(std::make_pair(name, nullptr));
			if (!ret.second)
				JSB_EXCEPT(name + STR(" is existed"));
			ret.first->second = new NormalFunction<F>(func);
		}

		template<class O, class F>
		void setKeyListener(const String& name, O* o, F func)
		{
			auto ret = mKeyListeners.insert(std::make_pair(name, nullptr));
			if (!ret.second)
				JSB_EXCEPT(name + STR(" is existed"));
			ret.first->second = new ClassMemberFunction<O,F>(o,func);
		}

		void removeListener(const String& name);

		void setAttribute(WORD attr);
		void setAttribute(size_t x, size_t y, WORD attr);
		void setLineAttribute(size_t x, size_t y, size_t length, WORD attr);
		void setLineAttribute(size_t x, size_t y, const std::vector<WORD>& attrs);
		void setLineAttribute(size_t x, size_t y, const WORD* attrs, size_t len);


		WORD getAttribute(size_t x, size_t y);
		void getLineAttribute(size_t x, size_t y, WORD* buffer, size_t size);

		RECT getConsoleRect();

		COORD getCursorPosition();
		void setCursorPosition(int x, int y);
		void setCursorVisible(bool v);

		void update();

	private:
		std::map<String, KeyEventListener*> mKeyListeners;
	};

}

#endif