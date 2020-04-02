#ifndef _FUSIN_LOG_H
#define _FUSIN_LOG_H

#include <iostream>

namespace Fusin
{

	class Log
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
			std::wcout << a;
			return *this;
		}
	};


}

#endif