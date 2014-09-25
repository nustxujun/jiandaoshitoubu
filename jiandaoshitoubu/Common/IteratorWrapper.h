#ifndef _IteratorWrapper_H_
#define _IteratorWrapper_H_

namespace JSB
{
	template<class T>
	class IteratorWrapper
	{
	public:
		typedef typename T::iterator Iter;
	public:
		IteratorWrapper(Iter begin, Iter end) :
			mBeg(begin),mCur(begin), mEnd(end)
		{
		}

		bool hasElements()const
		{
			return mCur != mEnd;
		}

		Iter current()const
		{
			return mCur;
		}

		Iter end()const
		{
			return mEnd;
		}

		Iter begin()const
		{
			return mBeg;
		}

		//Iter next()
		//{
		//	return ++mCur;
		//}

		Iter operator++()
		{
			return ++mCur;
		}

		Iter operator++(int)
		{
			return mCur++;
		}

		void reset()
		{
			mCur = mBeg;
		}
	private:
		Iter mBeg;
		Iter mCur;
		Iter mEnd;

	};

}

#endif