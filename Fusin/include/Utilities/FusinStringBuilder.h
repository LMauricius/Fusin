#ifndef _FUSIN_STRING_BUILDER_H
#define _FUSIN_STRING_BUILDER_H

#include "FusinPrerequisites.h"

namespace Fusin
{

	template<class CharT>
	class GenericStringBuilder
	{
	public:
		using StreamT = std::basic_stringstream<CharT, std::char_traits<CharT>, std::allocator<CharT> >;
		using StringT = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> >;

		template<class _T>
		GenericStringBuilder<CharT>& operator << (const _T& t)
		{
			stream << t;
			return *this;
		}

        inline StringT get() const
        {
            return stream.str();
        }

        inline operator StringT() const
        {
            return get();
        }

        StreamT stream;
	};

	// char-based
	using CStringBuilder = GenericStringBuilder<char>;

	// wchar-based
	using WStringBuilder = GenericStringBuilder<wchar_t>;

	// Fusin Char-based
	using StringBuilder = GenericStringBuilder<Char>;

}

#endif