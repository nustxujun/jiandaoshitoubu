#include "LogicHUD.h"
#include "EventManager.h"
#include "Events.h"
#include "DataStream.h"
using namespace JSB;

static const Char* KEY_EVENT_HANDLE = STR("KeyEventHandle");

CommonOutput::CommonOutput(Console* c) :HUD(c)
{
	RECT con = mConsole->getConsoleRect();
	RECT rect = { 0, 0, con.right / 2, con.bottom / 2 };
	setRect(rect);
	clear();
}

void CommonOutput::write(const String& str)
{
	if (mEndl)
	{
		std::vector<WORD> attrs(mRect.right);
		mConsole->getLineAttribute(mRect.left, mRect.top, &attrs[0], mRect.right);
		for (auto& i : attrs)
			i = i& ~(FOREGROUND_INTENSITY);
		mConsole->setLineAttribute(mRect.left, mRect.top, attrs);
		for (int i = mRect.top + mRect.bottom - 1; i >= mRect.top; --i)
		{
			String str = mConsole->getLine(mRect.left, i, mRect.right);
			mConsole->getLineAttribute(mRect.left, i, &attrs[0], mRect.right);
			//mConsole->setLineAttribute(mRect.left, i, mRect.right, 0);
			//mConsole->fill(mRect.left, i, ' ', mRect.right);

			mConsole->fill(mRect.left, i + 1, ' ', mRect.right);
			mConsole->setLineAttribute(mRect.left, i + 1, mRect.right, 0);

			mConsole->write(mRect.left, i + 1, str);
			mConsole->setLineAttribute(mRect.left, i + 1, attrs);

		}

		mCurpos.X = (SHORT)mRect.left;
		mCurpos.Y = (SHORT)mRect.top;
		mEndl = false;

		mConsole->fill(mCurpos.X, mCurpos.Y, ' ', mRect.right);
		mConsole->setLineAttribute(mCurpos.X, mCurpos.Y, mRect.right, 0);
	}
	
	HUD::operator<<( HUD::TEXT_LIGHT);

	mConsole->write(mCurpos.X, mCurpos.Y, str);
	mConsole->setLineAttribute(mCurpos.X, mCurpos.Y, str.size(), mLastAttr);

	mCurpos.X += str.size();

}

HUD& CommonOutput::operator << (const HUD::Operator& val)
{
	if (val == HUD::ENDL)
		mEndl = true;
	else
		HUD::operator<<(val);
	return *this;
}


CommonInput::CommonInput(Console* c)
	:HUD(c)
{
	RECT con = mConsole->getConsoleRect();
	RECT rect = { 0, con.bottom, con.right , 1 };
	setRect(rect);
	clear();

}

CommonInput::~CommonInput()
{
	mConsole->removeListener(STR("CommonInput"));
}

void CommonInput::write(const String& str)
{
	HUD::write(str);

	mConsole->setCursorPosition(mCurpos.X, mCurpos.Y);
}

String CommonInput::read()
{
	String tmp = HUD::read();
	clear();
	return tmp;
}


void CommonInput::clear()
{
	HUD::clear();

	*this << '>';
}

void CommonInput::setFocus(bool v)
{
	if (v == mFocus) return;

	HUD::setFocus(v);
	if (v)
	{
		auto lambda = [this](const KEY_EVENT_RECORD& record)
		{
			if (record.bKeyDown == 1) return;
			Char c = record.uChar.AsciiChar;
			static String cmd;
			if (c == '\r')
			{
				clear();
				processInput(cmd);
				cmd.clear();
			}
			else
			{
				*this << c;
				cmd += c;
			}
		};

		mConsole->setKeyListener(KEY_EVENT_HANDLE, lambda);
	}
	else
	{
		mConsole->removeListener(KEY_EVENT_HANDLE);
	}
}

void CommonInput::processInput(const String& str)
{
	class InputPara:public Parameter
	{
	public:
		String str;
	}input;
	input.str = str;
	EventManager::getSingleton().notifyEvent(EVENT_C_COMMON_INPUT, input);
}

Menu::Menu(Console* c) : HUD(c)
{
	RECT con = mConsole->getConsoleRect();
	const int height = 5;
	RECT rect = { 0, con.bottom - height , con.right, height };
	setRect(rect);
	clear();


	EventManager::getSingleton().registerEvent(EVENT_C_COMMON_INPUT, this, &Menu::processCommand);
}

Menu::~Menu()
{
	
}

void Menu::setFocus(bool v)
{
	if (v == mFocus) return;
	HUD::setFocus(v);
	if (v)
		mConsole->setKeyListener(KEY_EVENT_HANDLE,this, &Menu::processKeyEvent);
	else
		mConsole->removeListener(KEY_EVENT_HANDLE);
}

void Menu::processKeyEvent(const KEY_EVENT_RECORD& r)
{}

void Menu::processCommand(Parameter& para)
{
	class InputCommand : public Parameter
	{
	public :
		String cmd;
	};

	InputCommand& cmd = (InputCommand&)para;

	if (mListener != nullptr)
	{
		(*mListener)(cmd.cmd);
	}

	//*this << DEFAULT <<cmd.cmd << ENDL;
}

CommonView::CommonView(Console* c) :HUD(c)
{
	RECT con = mConsole->getConsoleRect();
	RECT rect = { con.right / 2, 0, con.right / 2, con.bottom / 2 };
	setRect(rect);
	clear();
	
}

CommonView::~CommonView()
{

}