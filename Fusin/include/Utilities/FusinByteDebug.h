#ifndef _FUSIN_BYTE_DEBUG_H
#define _FUSIN_BYTE_DEBUG_H

#include "FusinPrerequisites.h"
#include <sstream>

namespace Fusin
{

	inline String byteString(void* begin, unsigned int byteNum)
	{
		unsigned char a, b;
		String str;
		for (int i = 0; i < byteNum; i++)
		{
			a = *((unsigned char*)begin + i) / 16;
			b = *((unsigned char*)begin + i) % 16;

			if (a > 9) str += (Char)a - 10 + 65;
			else str += (Char)a + 48;
			if (b > 9) str += (Char)b - 10 + 65;
			else str += (Char)b + 48;

			str += ' ';
		}
		return str;
	}

	inline String byteStringCondensed(void* begin, unsigned int byteNum)
	{
		unsigned char a, b;
		String str;
		for (int i = 0; i < byteNum; i++)
		{
			a = *((unsigned char*)begin + i) / 16;
			b = *((unsigned char*)begin + i) % 16;

			if (a > 9) str += (Char)a - 10 + 65;
			else str += (Char)a + 48;
			if (b > 9) str += (Char)b - 10 + 65;
			else str += (Char)b + 48;
		}
		return str;
	}

	inline String byteStringNumerated(void* begin, unsigned int byteNum)
	{
		unsigned char a, b;
		String str1, str2;
		int space;
		String sepStr;
		if (byteNum < 100)
		{
			space = 3;
			sepStr = L" ";
		}
		else if (byteNum < 1000)
		{
			space = 4;
			sepStr = L"  ";
		}
		else if (byteNum < 10000)
		{
			space = 5;
			sepStr = L"   ";
		}

		for (int i = 0; i < byteNum; i++)
		{
			std::wstringstream ss;
			ss << i;
			str1 += ss.str();
			for (int j = 0; j < space - 1 - (i >= 10) - (i >= 100) - (i >= 1000) - (i >= 10000); j++) str1 += ' ';

			a = *((unsigned char*)begin + i) / 16;
			b = *((unsigned char*)begin + i) % 16;

			if (a > 9) str2 += (Char)a - 10 + 65;
			else str2 += (Char)a + 48;
			if (b > 9) str2 += (Char)b - 10 + 65;
			else str2 += (Char)b + 48;

			str2 += sepStr;
		}
		str1 += '\n';
		return str1 + str2;
	}

	inline String bitString(void* begin, unsigned int byteNum)
	{
		unsigned char a, b;
		String str;
		for (int i = 0; i < byteNum; i++)
		{
			for (int j = 7; j >= 0; j--)
			{
				if (*((unsigned char*)begin + i) & 1 << j) str += '1';
				else str += '0';
			}
		}
		return str;
	}

}

#endif