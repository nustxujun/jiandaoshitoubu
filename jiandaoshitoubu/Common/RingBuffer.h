#ifndef _RingBuffer_H_
#define _RingBuffer_H_

#include "Common.h"

namespace JSB
{
	class RingBuffer
	{
	public:
		struct AllocBuffer
		{
			void* buffer;
			size_t size;
		};
	public:
		RingBuffer(size_t cap) : mSize(0)
		{
			mData = mHead = mLast = new char[cap];
			mTail = mData + cap;
		}

		~RingBuffer()
		{
			delete mData;
			mData = mHead = mLast = mTail = 0;
		}

		void read(void* buffer, size_t bsize)
		{
			Assert(bsize <= size());

			size_t part1 = mTail - mHead;
			part1 = (part1 < bsize) ? part1 : bsize;
			size_t part2 = bsize - part1;
			memcpy(buffer, mHead, part1);
			mHead += part1;
			if (mHead == mTail)
			{
				memcpy((char*)buffer + part1, mData, part2);
				mHead = mData + part2;
			}
			mSize -= bsize;
		}

		AllocBuffer alloc(size_t size)
		{
			AllocBuffer ab;
			ab.buffer = mLast;

			ab.size = min((size_t)(mTail - mLast), min(capacity() - mSize, size));
			return ab;
		}

		void adjust(size_t count)
		{
			mLast += count;
			if (mLast == mTail)
				mLast = mData;

			mSize += count;
			Assert(mSize <= capacity());
		}

		void free(size_t count)
		{
			Assert(count <= mSize);
			mHead += count;
			if (mTail <= mHead )
			{
				mHead = mData + (mHead - mTail);
			}

			mSize -= count;
		}

		AllocBuffer data()//not all
		{
			AllocBuffer ab;
			ab.buffer = mHead;
			ab.size = min((size_t)(mTail - mHead), mSize);
			return ab;
		}

		size_t size()
		{
			return mSize;
		}
		size_t capacity()
		{
			return mTail - mData;
		}

	private:
		char* mData;
		char* mHead;
		char* mLast;
		char* mTail;
		size_t mSize;
	};

}

#endif