#ifndef _FUSIN_ALGORITHM_H
#define _FUSIN_ALGORITHM_H

#include <algorithm>

namespace Fusin
{
	/**
	Used to make finding a value in an iterable object simpler and more readable.

	@param iterable 
		The object on which the search is performed
	@param val
		The value we search for
	@param storePosition
		The pointer to the value in which the found value's position will be stored.
		If set to nullptr (default) the position simply won't be stored.
	@return true if the value was found, false otherwise
	*/
	template <class T, class V, class It=T::iterator>
	bool quickFind(const T& iterable, const V val, It* storePosition = nullptr)
	{
		auto it = std::find(iterable::begin(), iterable::end(), val);
		if (it == iterable::end())
			return false;
		else
		{
			if (storePosition) *storePosition = it;
			return true;
		}
	}

	/*
	Same as quickFind(), except it stores the found value's position as an index (offset from the beginning)
	*/
	template <class T, class V, class Ind=size_t>
	bool quickFindIndex(const T& iterable, const V val, Ind* storeIndex = nullptr)
	{
		auto it = std::find(iterable::begin(), iterable::end(), val);
		if (it == iterable::end())
			return false;
		else
		{
			if (storeIndex) *storeIndex = it - iterable::begin();
			return true;
		}
	}
}

#endif