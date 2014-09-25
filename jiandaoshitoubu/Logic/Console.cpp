#include "Console.h"

using namespace JSB;

void Console::write(const Char* buffer, size_t size)
{
	DWORD count = 0;
	::WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), buffer, size, &count, 0);

}

void Console::write(const String& str)
{
	write(str.data(), str.size());
}

void Console::write(size_t x, size_t y, const Char* buffer, size_t size)
{
	DWORD count = 0;
	COORD coord = { x, y };
	::WriteConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), buffer, size, coord, &count);
}

void Console::write(size_t x, size_t y, const String& str)
{
	write(x, y, str.data(), str.size());
}

void Console::fill(size_t x, size_t y, Char c, size_t len)
{
	DWORD count = 0;
	COORD coord = { x, y };
	::FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), c, len, coord, &count);

}

bool Console::getKeyEvent(KEY_EVENT_RECORD& record)
{
	DWORD count;
	INPUT_RECORD irInBuf;
	if (!PeekConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &irInBuf, 1, &count) || 
		count == 0)
		return false;
	if (!ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &irInBuf, 1, &count))
		return false;
	if (irInBuf.EventType != KEY_EVENT)
		return false;
	record = irInBuf.Event.KeyEvent;
	return true;
}


Char Console::getChar()
{
	KEY_EVENT_RECORD record; 
	while (true)
	{
		if (!getKeyEvent(record))
			continue;

		if (record.bKeyDown == TRUE)
			continue;
		WORD key = record.uChar.AsciiChar;
		if (key != 0)
			return (Char)key;
		else
			continue;
	}

}

String Console::read(Callback* callback)
{
	String result;
	while (true)
	{
		WORD key = getChar();
		if (key == '\r')
			return result;
		else if (key >= 32 && key <= 126)
		{
			result = result + (Char)key;
			(*callback)((Char)key);
		}
	}
}

void Console::setAttribute(WORD attr)
{
	::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), attr);
	//::SetConsoleTextAttribute(::GetStdHandle(STD_INPUT_HANDLE), attr);
}

void Console::setAttribute(size_t x, size_t y, WORD attr)
{
	COORD coord = { x, y };
	DWORD count = 0;
	::WriteConsoleOutputAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), &attr, 1, coord, &count);
}
void Console::setLineAttribute(size_t x, size_t y, size_t length, WORD attr)
{
	std::vector<WORD> attrs;
	attrs.reserve(length);
	for (size_t i = 0; i < length; ++i)
		attrs.push_back(attr);
	setLineAttribute(x, y, attrs);
}

void Console::setLineAttribute(size_t x, size_t y, const std::vector<WORD>& attrs)
{
	setLineAttribute(x, y, attrs.data(), attrs.size());
}

void Console::setLineAttribute(size_t x, size_t y, const WORD* attrs, size_t len)
{
	COORD coord = { x, y };
	DWORD count = 0;
	::WriteConsoleOutputAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), attrs, len, coord, &count);

}

WORD Console::getAttribute(size_t x, size_t y)
{
	WORD attr = 0;
	COORD coord = { x, y };
	DWORD count = 0;
	::ReadConsoleOutputAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), &attr, 1, coord, &count);
	return attr;

}


void Console::getLineAttribute(size_t x, size_t y, WORD* buffer, size_t size)
{
	COORD coord = { x, y };
	DWORD count = 0;
	::ReadConsoleOutputAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), buffer, size, coord, &count);
}


RECT Console::getConsoleRect()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	RECT rect;
	rect.left = csbi.srWindow.Left;
	rect.top = csbi.srWindow.Top;
	rect.right = csbi.srWindow.Right - csbi.srWindow.Left;
	rect.bottom = csbi.srWindow.Bottom - csbi.srWindow.Top;
	return rect;
}

void Console::setCursorPosition(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

COORD Console::getCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bInfo);
	return bInfo.dwCursorPosition;
}


void Console::setCursorVisible(bool v)
{
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	cci.bVisible = v;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	//GetConsoleCursorInfo(GetStdHandle(STD_ERROR_HANDLE), &cci);
	//cci.bVisible = false;
	//SetConsoleCursorInfo(GetStdHandle(STD_ERROR_HANDLE), &cci);
}

void Console::removeListener(const String& name)
{
	auto ret = mKeyListeners.find(name);
	if (ret == mKeyListeners.end())
		return;
	delete ret->second;
	mKeyListeners.erase(ret);
}

void Console::update()
{
	KEY_EVENT_RECORD key;
	while (getKeyEvent(key))
	{
		auto copy = mKeyListeners;
		for (auto i : copy)
		{
			(*i.second)(key);
		}
	}
}

String Console::getLine(size_t x, size_t y, size_t length)
{
	Char* buff = new Char[length];
	COORD coord = { x, y };
	DWORD count = 0;
	::ReadConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), buff, length, coord, &count);
	String ret(buff, length);
	delete buff;
	return ret;
}
