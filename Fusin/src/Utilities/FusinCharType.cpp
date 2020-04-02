#include "FusinCharType.h"

namespace Fusin
{
	CharType getCharType(Char c)
	{
		switch (c)
		{
		case 10:
		case 13:
			return CT_NEW_LINE;
		case ' ':
			return CT_SINGLE_SPACE;
		case '\t':
			return CT_TAB;
		case '_':
			return CT_UNDERSCORE;
		case '\\':
		case '/':
			return CT_SYMBOL_FILEPATH_SEPARATOR | CT_SYMBOL_MATH;
		case '.':
		case '+':
		case '-':
		case '=':
		case '(':
		case ')':
		case '[':
		case ']':
		case '{':
		case '}':
		case '^':
		case '%':
		case '!':
		case ',':
			return CT_SYMBOL_FILENAME | CT_SYMBOL_MATH;
		case ':':
		case '*':
		case '>':
		case '<':
			return CT_SYMBOL_MATH;
		}

		if (c < 32 || c == 127)
			return CT_CONTROL;
		if (c >= 48 && c <= 57)
			return CT_DIGIT;
		if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
			return CT_LETTER_ENG;
		if (c >= 33 && c <= 126)
			return CT_SYMBOL_OTHER;

		return CT_CHAR_EXT;
	}

}