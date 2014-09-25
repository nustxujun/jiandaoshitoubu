#include "HUD.h"

using namespace JSB;

HUD::HUD(Console* con) : mConsole(con)
{

}

HUD::~HUD()
{
	setFocus(false);

}

HUD& HUD::operator<< (const Operator& val)
{
	switch (val)
	{
	case ENDL:
		{
			if (mCurpos.Y < mRect.top + mRect.bottom)
			{
				++mCurpos.Y;
				mCurpos.X = (SHORT)mRect.left;

			}
			else
			{
				std::vector<WORD> attrs(mRect.right);

				mConsole->setLineAttribute(mRect.left, mRect.top, attrs);
				for (int i =  1; i < mRect.bottom; ++i)
				{
					mConsole->setLineAttribute(mRect.left, mRect.top + i - 1, mRect.right, 0);
					mConsole->fill(mRect.left, mRect.top + i - 1, ' ', mRect.right);

					String str = mConsole->getLine(mRect.left, mRect.top + i, mRect.right);
					mConsole->getLineAttribute(mRect.left, mRect.top + i, &attrs[0], mRect.right);

					mConsole->write(mRect.left, mRect.top + i - 1, str);
					mConsole->setLineAttribute(mRect.left, mRect.top + i - 1, attrs);
				}

				mConsole->fill(mRect.left, mRect.top + mRect.bottom - 1, ' ', mRect.right);
				mConsole->setLineAttribute(mRect.left, mRect.top + mRect.bottom - 1, mRect.right, 0);
				mCurpos.X = 0;
			}
		}
		break;
	}
	return *this;
}

HUD& HUD::operator << (const Attribute& val)
{
	int len = mRect.right - (mCurpos.X - mRect.left);
	convertAttribute(mLastAttr, val);
	//输入时再设置
	//mConsole->setLineAttribute(mCurpos.X, mCurpos.Y, len, mLastAttr);
	return *this;
}	

void HUD::convertAttribute(WORD& att, Attribute a)
{

	switch (a)
	{
	case DEFAULT:
		att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
	case TEXT_BLUE:
		att = (att | FOREGROUND_BLUE) & ~(FOREGROUND_GREEN | FOREGROUND_RED);
		break;
	case TEXT_GREEN:
		att = (att | FOREGROUND_GREEN) & ~(FOREGROUND_BLUE | FOREGROUND_RED);
		break;
	case TEXT_RED:
		att = (att | FOREGROUND_RED) & ~(FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case TEXT_YELLOW:
		att &= ~FOREGROUND_BLUE;
		break;
	case TEXT_PINK:
		att &= ~FOREGROUND_GREEN;
		break;
	case TEXT_CYAN:
		att &= ~FOREGROUND_RED;
		break;
	case TEXT_LIGHT:
		att |= FOREGROUND_INTENSITY;
		break;
	case TEXT_DARK:
		att = att & ~(FOREGROUND_INTENSITY);
		break;
	case BG_BLUE:
		att = (att | BACKGROUND_BLUE) & ~(BACKGROUND_GREEN | BACKGROUND_RED);
		break;
	case BG_GREEN:
		att = (att | BACKGROUND_GREEN) & ~(BACKGROUND_BLUE | BACKGROUND_RED);
		break;
	case BG_RED:
		att = (att | BACKGROUND_RED) & ~(BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	case BG_YELLOW:
		att &= ~BACKGROUND_BLUE;
		break;
	case BG_PINK:
		att &= ~BACKGROUND_GREEN;
		break;
	case BG_CYAN:
		att &= ~BACKGROUND_RED;
		break;
	case BG_LIGHT:
		att |= BACKGROUND_INTENSITY;
		break;
	}
}


void HUD::drawBox(const RECT& rect)
{
	//bool bSingle = true;
	//char chBox[6];
	//{
	//	chBox[0] = 'F'; // 左上角点
	//	chBox[1] = '7'; // 右上角点
	//	chBox[2] = 'L'; // 左下角点
	//	chBox[3] = 'd'; // 右下角点
	//	chBox[4] = '-'; // 水平
	//	chBox[5] = '|'; // 坚直
	//}


	//mConsole.fill(rect.left + 1, rect.top, chBox[4], rect.right - 1);
	//mConsole.fill(rect.left + 1, rect.top + rect.bottom, chBox[4], rect.right - 1);
	//
	//for (int i = 1; i < rect.bottom; ++i)
	//	mConsole.fill(rect.left, rect.top + i, chBox[5], 1);

	//for (int i = 1; i < rect.bottom; ++i)
	//	mConsole.fill(rect.left + rect.right, rect.top + i, chBox[5], 1);


}



void HUD::clear()
{

	mCurpos.X = (SHORT)mRect.left;
	mCurpos.Y = (SHORT)mRect.top;
	convertAttribute(mLastAttr, DEFAULT);
	for (int i = mRect.bottom - 1; i >= 0; --i)
	{
		mConsole->fill(mRect.left, mRect.top + i, ' ', mRect.right);
		mConsole->setLineAttribute(mRect.left, mRect.top + i,  mRect.right, mLastAttr);

	}
	

}


void HUD::write(const String& str)
{
	mConsole->setLineAttribute(mCurpos.X, mCurpos.Y, str.size(), mLastAttr);
	mConsole->write(mCurpos.X, mCurpos.Y, str);

	mCurpos.X += str.size();

}

String HUD::read()
{
	auto lambda = [this](Char c)
	{
		*this << c;
	};
	return mConsole->read(lambda);
}


void HUD::setRect( const RECT& rect)
{
	mRect = rect;
}

const RECT& HUD::getRect()
{
	return mRect;
}


void HUD::setFocus(bool val)
{
	mFocus = val;
}
