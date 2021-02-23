#ifndef _FUSIN_LOG_H
#define _FUSIN_LOG_H

#include <iostream>
#include <ostream>

namespace Fusin
{

	class _FUSIN_EXPORT Log
	{
	public:
		static inline Log& singleton()
		{
			static Log l;
			return l;
		}

		template<class T>
		Log& operator << (T a)
		{
			if (mStream)
				(*mStream) << a;
			return *this;
		}

		inline void pipe(std::wostream& target)
		{
			mStream = &target; 
		}

	private:
		std::wostream *mStream;

		inline Log():
			mStream(nullptr)
		{}
	};


}

#endif