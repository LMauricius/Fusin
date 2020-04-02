#ifndef _FUSIN_CHAR_TYPE_H
#define _FUSIN_CHAR_TYPE_H

#include "FusinPrerequisites.h"

namespace Fusin
{
	/*
	Useful for restricting keyboard input to selected types of characters.
	*/
	typedef unsigned int CharType;

	const CharType CT_CONTROL = 1;// Control characters, ASCII <= 31 and ASCII 127
	const CharType CT_SYMBOL_FILENAME = 1 << 1;// Symbols that can be included in filenames (not file paths)
	const CharType CT_SYMBOL_FILEPATH_SEPARATOR = 1 << 2;// Directory separators in file paths
	const CharType CT_SYMBOL_MATH = 1 << 3;// Math symbols (ASCII)
	const CharType CT_SYMBOL_OTHER = 1 << 4;// Other symbols
	const CharType CT_SYMBOL = CT_SYMBOL_FILENAME | CT_SYMBOL_FILEPATH_SEPARATOR | CT_SYMBOL_MATH | CT_SYMBOL_OTHER;// All symbols
	const CharType CT_SINGLE_SPACE = 1 << 5;// A common space (' ') character
	const CharType CT_TAB = 1 << 6;// Tab space character
	const CharType CT_NEW_LINE = 1 << 7;// New line character
	const CharType CT_SPACE = CT_SINGLE_SPACE | CT_TAB | CT_NEW_LINE;// All space characters
	const CharType CT_UNDERSCORE = 1 << 8;// Underscore, duh ('_')
	const CharType CT_DIGIT = 1 << 9;// Digits 0-9
	const CharType CT_LETTER_ENG = 1 << 10;// Letters of the english alphabet
	const CharType CT_CHAR_EXT = 1 << 11;// Extended character set, ASCII > 127, mostly non-english letters in non-ASCII formats
	const CharType CT_LETTER = CT_LETTER_ENG | CT_CHAR_EXT;// English letters + extended character set, so mostly characters from all alphabets if unicode
	const CharType CT_PRINTABLE = CT_LETTER | CT_DIGIT | CT_SYMBOL | CT_UNDERSCORE;// All printable (visible) characters
	const CharType CT_PRINTABLE_ENG = CT_LETTER_ENG | CT_DIGIT | CT_SYMBOL | CT_UNDERSCORE;// All english printable (visible) characters
	const CharType CT_TEXT_LINE = CT_PRINTABLE | CT_SINGLE_SPACE;// All printable characters, including single space
	const CharType CT_TEXT_LINE_ENG = CT_PRINTABLE_ENG | CT_SINGLE_SPACE;// All printable english characters, including single space
	const CharType CT_TEXT = CT_TEXT_LINE | CT_TAB | CT_NEW_LINE;// Multi-line text
	const CharType CT_TEXT_ENG = CT_TEXT_LINE_ENG | CT_TAB | CT_NEW_LINE;// Multi-line english text
	const CharType CT_IDENTIFIER = CT_LETTER_ENG | CT_DIGIT | CT_UNDERSCORE;// Most common definition of an identifier in programming (english letters, digits and underscore)
	const CharType CT_IDENTIFIER_EXT = CT_LETTER | CT_DIGIT | CT_UNDERSCORE;// Identifier, except with non-english letters
	const CharType CT_FILENAME_NO_SPACE = CT_IDENTIFIER_EXT | CT_SYMBOL_FILENAME;// Characters that can be included in filenames, WITHOUT spaces (NOT file paths)
	const CharType CT_FILENAME_NO_SPACE_ENG = CT_IDENTIFIER | CT_SYMBOL_FILENAME;// English characters that can be included in filenames, WITHOUT spaces (NOT file paths)
	const CharType CT_FILENAME = CT_FILENAME_NO_SPACE | CT_SINGLE_SPACE;// Characters that can be included in filenames, INCLUDING spaces (NOT file paths)
	const CharType CT_FILENAME_ENG = CT_FILENAME_NO_SPACE_ENG | CT_SINGLE_SPACE;// English characters that can be included in filenames, INCLUDING spaces (NOT file paths)
	const CharType CT_FILEPATH_NO_SPACE = CT_FILENAME_NO_SPACE | CT_SYMBOL_FILEPATH_SEPARATOR;// Characters that can be included in file paths, WITHOUT spaces
	const CharType CT_FILEPATH_NO_SPACE_ENG = CT_FILENAME_NO_SPACE_ENG | CT_SYMBOL_FILEPATH_SEPARATOR;// English characters that can be included in file paths, WITHOUT spaces
	const CharType CT_FILEPATH = CT_FILENAME | CT_SYMBOL_FILEPATH_SEPARATOR;// Characters that can be included in file paths, INCLUDING spaces
	const CharType CT_FILEPATH_ENG = CT_FILENAME_ENG | CT_SYMBOL_FILEPATH_SEPARATOR;// English characters that can be included in file paths, INCLUDING spaces
	const CharType CT_MATH = CT_IDENTIFIER_EXT | CT_SINGLE_SPACE | CT_SYMBOL_MATH;// Characters that can be found in math expressions (Only ASCII symbols)
	const CharType CT_MATH_ENG = CT_IDENTIFIER | CT_SINGLE_SPACE | CT_SYMBOL_MATH;// Characters that can be found in math expressions, without non-english letters (Only ASCII symbols)
	const CharType CT_ALL = -1; // All flags

	CharType getCharType(Char c);
}

#endif