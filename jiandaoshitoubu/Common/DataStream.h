#ifndef _Message_H_
#define _Message_H_

#include "Common.h"

#define DEFAULT_MSG_SIZE 32
#define ALIGN(s) (((s) + 3 ) & ~3)

namespace JSB
{
	class DataStream
	{
	public :
		DataStream(size_t size = DEFAULT_MSG_SIZE)
		{
			size_t realsize = ALIGN(size);
			mRead = mLast = mFirst = new char[realsize];
			mTail = mFirst + realsize;
			
		}

		DataStream(const void* buffer, size_t size)
		{
			size_t realsize = ALIGN(size);
			mRead = mFirst = new char[realsize];
			mTail = mFirst + realsize;
			memcpy(mFirst, buffer, size);
			mLast = mFirst + size;
		}

		~DataStream()
		{
			delete mFirst;
		}

		void write(const void* buffer, size_t count)
		{
			reserve(size() + count);

			memcpy(mLast, buffer, count);
			mLast += count;
		}

		void read(void* buffer, size_t count)
		{
			memcpy(buffer, mRead, count);
			skipR(count);
		}
		
		template<class T>
		DataStream& operator << (const T& val)
		{
			write(&val, sizeof(T));
			return *this;
		}

		template<class T>
		DataStream& operator << (const T* val) = delete;

		DataStream& operator << (const String& str)
		{
			size_t len = str.size() * sizeof(Char);
			(*this) << len;
			write(str.c_str(), len);
			return *this;
		}

		DataStream& operator << (const Char* str)
		{
			String tmp(str);
			return *this << tmp;
		}

		template<class T>
		DataStream& operator >> (T& val)
		{
			read(&val, sizeof(T));
			return *this;
		}

		DataStream& operator >> (String& str)
		{
			size_t len;
			(*this) >> len;

			Assert(mRead + len <= mLast);//check range

			str.swap(String((Char*)mRead,len));
			skipR(len);
			return *this;
		}

		template<class T>
		DataStream& operator >> (const T*& val)
		{
			val = (const T*)mRead;
			skip(sizeof(T));
			return *this;
		}

		template<class T>
		DataStream& operator >> (T*& val) = delete;


		void reserve(size_t count)
		{
			if (mFirst + count <= mTail)
				return;
			size_t oldsize = mLast - mFirst;
			size_t newcap = ALIGN(count << 1);
			char* tmp = new char[newcap];
			memcpy(tmp, mFirst, oldsize);
			delete mFirst;
			mRead = tmp + (mRead - mFirst);
			mLast = tmp + oldsize;
			mTail = tmp + newcap;
			mFirst = tmp;

		}

		size_t size()const
		{
			return mLast - mFirst;
		}

		const char* data()const
		{
			return mFirst;
		}
		
		void clear()
		{
			mLast = mRead = mFirst;
		}

		void reset()
		{
			mRead = mFirst;//only read;
		}

		void skipR(size_t count)
		{
			Assert(mRead + count <= mLast);//check range

			mRead += count;
		}

		void skipW(size_t count)
		{
			Assert(mLast + count <= mTail);//check range

			mLast += count;
		}

		size_t tell()const
		{
			return mLast - mRead;
		}

		bool eof()const
		{
			return mRead == mLast;
		}

		DataStream(const DataStream& msg) = delete;
		void operator=(const DataStream& msg) = delete;

	private:
		char* mFirst;
		char* mLast;
		char* mTail;
		char* mRead;
	};
}

#endif